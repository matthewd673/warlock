#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Ray.h"

struct CollPoint {
    float x;
    float y;
};

CollPoint new_CollPoint(float x, float y) {
    CollPoint this = (CollPoint)malloc(sizeof(struct CollPoint));
    if (this == NULL) return NULL;

    this->x = x;
    this->y = y;

    return this;
}

float CollPoint_GetX(CollPoint p) { return p->x; }
float CollPoint_GetY(CollPoint p) { return p->y; }
void CollPoint_SetX(CollPoint p, float x) {
    p->x = x;
}
void CollPoint_SetY(CollPoint p, float y) {
    p->y = y;
}

//https://gamedev.stackexchange.com/a/117294
int Ray_RRCollision(CollPoint point,
                    float aX1, float aY1, float aX2, float aY2,
                    float bX1, float bY1, float bX2, float bY2) {
    
    float r = ((aY1-bY1)*(bX2-bX1)-(aX1-bX1)*(bY2-bY1)) / 
              ((aX2-aX1)*(bY2-bY1)-(aY2-aY1)*(bX2-bX1));
    float s = ((aY1-bY1)*(aX2-aX1)-(aX1-bX1)*(aY2-aY1)) /
              ((aX2-aX1)*(bY2-bY1)-(aY2-aY1)*(bX2-bX1));

    float iX = aX1 + r*(aX2-aX1);
    float iY = aY1 + r*(aY2-aY1);

    if (r < 0 || r > 1 || s < 0 || s > 1) //intersection doesn't exist
        return 0;
    
    point->x = iX;
    point->y = iY;

    return 1;
}

float Ray_DistBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Ray_CastFromCamera(float *distv, int *mapv, int *texv, Camera cam, World world, SDL_PixelFormat *format) {

    for (int i = 0; i < Camera_GetHalfRays(cam) * 2; i++) {
        float angle = Camera_GetSightRays(cam)[i];
        float rX1 = Camera_GetX(cam);
        float rY1 = Camera_GetY(cam);
        float rX2 = rX1 + cos(angle)*Camera_GetSightMag(cam);//*cos(angle - Camera_GetAngle(cam));
        float rY2 = rY1 + sin(angle)*Camera_GetSightMag(cam);//*cos(angle - Camera_GetAngle(cam));
        
        CollPoint currentColl = new_CollPoint(0, 0);
        float nearestDist = Camera_GetSightMag(cam);
        int nearestMap = 0;
        int nearestTex = 0;
        //TODO: inefficient for large worlds
        for (int j = 0; j < World_GetWallCt(world); j++) {
            Wall w = World_GetWalls(world)[j];
            
            CollPoint c = new_CollPoint(0, 0);
            if (!Ray_RRCollision(c,
                rX1, rY1, rX2, rY2,
                Wall_GetX1(w), Wall_GetY1(w), Wall_GetX2(w), Wall_GetY2(w)))
            {
                free(c);
                continue;
            }

            float dist = Ray_DistBetweenPoints(rX1, rY1, c->x, c->y)*cos(Camera_GetAngle(cam)-angle);// * (1-cos(Camera_GetAngle(cam)-angle));
            
            if (nearestDist < 0 || dist < nearestDist) {
                nearestDist = dist;
                nearestMap = (int)Ray_DistBetweenPoints(c->x, c->y, Wall_GetX1(w), Wall_GetY1(w)) % 64;
                nearestTex = Wall_GetTextureId(w);
                free(currentColl);
                currentColl = c;
            }
            else {
                free(c);
            }
        }

        distv[i] = nearestDist;
        mapv[i] = nearestMap;
        texv[i] = nearestTex;
    }
}
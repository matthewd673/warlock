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

float Ray_DistBetweenPointsSq(float x1, float y1, float x2, float y2) {
    return pow(x2 - x1, 2) + pow(y2 - y1, 2);
}

//https://www.gamers.org/dEngine/rsc/usenet/comp.graphics.algorithms.faq
// float Ray_DistToLineSq(float pX, float pY,
//                      float x1, float y1, float x2, float y2) {
//     float lenSq = pow(x2-x1, 2) + pow(y2-y1, 2);

//     float r = ((y1-pY)*(y1-y2) - (x1-pX)*(x2-x1)) / lenSq;
//     float s = ((y1-pY)*(x2-x1) - (x1-pX)*(y2-y1)) / lenSq

//     float iX = x1 + r*(x2-x1);
//     float iY = y1 + r*(x2-x1);

// }

void Ray_CastFromCamera(float *distv, Camera cam, World world) {

    for (int i = 0; i < Camera_GetHalfRays(cam) * 2; i++) {
        float angle = Camera_GetSightRays(cam)[i];
        float rX1 = Camera_GetX(cam);
        float rY1 = Camera_GetY(cam);
        float rX2 = rX1 + cos(angle)*Camera_GetSightMag(cam);
        float rY2 = rY1 + sin(angle)*Camera_GetSightMag(cam);
        
        CollPoint currentColl = new_CollPoint(0, 0);
        float nearestDist = -1;
        //TODO: inefficient for large worlds
        for (int j = 0; j < World_GetWallCt(world); j++) {
            Wall w = World_GetWalls(world)[j];
            
            CollPoint c = new_CollPoint(0, 0);
            if (!Ray_RRCollision(c,
                rX1, rY1, rX2, rY2,
                Wall_GetX1(w), Wall_GetY1(w), Wall_GetX2(w), Wall_GetY2(w))) {
                continue;
            }

            float dist = Ray_DistBetweenPointsSq(rX1, rY1, c->x, c->y);
            if (nearestDist < 0 || dist < nearestDist) {
                nearestDist = dist;
                free(currentColl);
                currentColl = c;
            }
            else {
                free(c);
            }
        }

        distv[i] = nearestDist;
    }
}
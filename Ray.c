#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ray.h"
#include "camera.h"
#include "world.h"

struct Point {
    float x;
    float y;
};

Point new_Point(float x, float y) {
    Point this = (Point)malloc(sizeof(struct Point));
    if (this == NULL) {
        return NULL;
    }

    this->x = x;
    this->y = y;

    return this;
}

float Point_GetX(Point p) {
    return p->x;
}

float Point_GetY(Point p) {
    return p->y;
}

// https://gamedev.stackexchange.com/a/117294
int Ray_RRCollision(Point p,
                    float aX1, float aY1, float aX2, float aY2,
                    float bX1, float bY1, float bX2, float bY2) {

    float r = ((aY1-bY1)*(bX2-bX1)-(aX1-bX1)*(bY2-bY1)) /
              ((aX2-aX1)*(bY2-bY1)-(aY2-aY1)*(bX2-bX1));
    float s = ((aY1-bY1)*(aX2-aX1)-(aX1-bX1)*(aY2-aY1)) /
              ((aX2-aX1)*(bY2-bY1)-(aY2-aY1)*(bX2-bX1));

    float iX = aX1 + r*(aX2-aX1);
    float iY = aY1 + r*(aY2-aY1);

    if (r < 0 || r > 1 || s < 0 || s > 1) // intersection doesn't exist
        return 0;

    p->x = iX;
    p->y = iY;

    return 1;
}

float Ray_DistBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Ray_Cast(RayCamera cam, World world, float *disv, int *mapv, int *texv) {
    int halfRays = Camera_GetHalfRayc(cam);
    for (int i = 0; i < halfRays * 2; i++) {
        float angle = Camera_GetRayv(cam)[i];
        float rX1 = Camera_GetX(cam);
        float rY1 = Camera_GetY(cam);

        // float screenHalfLen = Camera_GetProjDist(cam) * tan(Camera_GetFOV(cam)/2.0);
        // float rayLen = screenHalfLen / halfRays;

        // float angle = atan((rayLen*i - screenHalfLen) / Camera_GetProjDist(cam));

        float rX2 = rX1 + cos(angle)*Camera_GetProjDist(cam);//*cos(angle - Camera_GetAngle(cam));
        float rY2 = rY1 + sin(angle)*Camera_GetProjDist(cam);//*cos(angle - Camera_GetAngle(cam));

        Point currentColl = new_Point(0, 0);
        float nearestDist = Camera_GetProjDist(cam);
        int nearestMap = 0;
        int nearestTex = 0;
        // TODO: inefficient for large worlds
        for (int j = 0; j < World_GetWallc(world); j++) {
            Wall w = World_GetWallv(world)[j];

            // attempt to find ray collision point
            Point c = new_Point(0, 0);
            if (!Ray_RRCollision(c,
                rX1, rY1, rX2, rY2,
                Wall_GetX1(w), Wall_GetY1(w), Wall_GetX2(w), Wall_GetY2(w)))
            {
                free(c);

                continue;
            }

            float dist = Ray_DistBetweenPoints(rX1, rY1, c->x, c->y);
            dist *= cos(angle - Camera_GetAngle(cam)); //correct for fisheye

            // found new closest wall
            if (nearestDist < 0 || dist < nearestDist) {
                nearestDist = dist;
                nearestMap = round(Ray_DistBetweenPoints(c->x, c->y, Wall_GetX1(w), Wall_GetY1(w)));
                nearestTex = Wall_GetTexi(w);
                free(currentColl);
                currentColl = c;
            }
            // new intersection is farther than the last
            else {
                free(c);
            }
        }

        disv[i] = nearestDist;
        mapv[i] = nearestMap;
        texv[i] = nearestTex;
    }
}
#include "camera.h"
#include "world.h"

typedef struct Point *Point;

Point new_Point(float x, float y);

float Point_GetX(Point p);
float Point_GetY(Point p);

int Ray_RRCollision(Point p,
                    float aX1, float aY1, float aX2, float aY2,
                    float bX1, float bY1, float bX2, float bY2);
float Ray_DistBetweenPoints(float x1, float y1, float x2, float y2);
void Ray_Cast(RayCamera cam, World world, float *disv, int *mapv, int *texv);
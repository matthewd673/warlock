#include "Camera.h"
#include "World.h"

typedef struct CollPoint *CollPoint;

CollPoint new_CollPoint(float x, float y);
float CollPoint_GetX(CollPoint p);
float CollPoint_GetY(CollPoint p);
void CollPoint_SetX(CollPoint p, float x);
void CollPoint_SetY(CollPoint p, float y);

int Ray_RRCollision(CollPoint point,
                    float aX1, float aY1, float aX2, float AY2,
                    float bX1, float bY1, float bX2, float BY2);

float Ray_DistBetweeenPoints(float x1, float y1, float x2, float y2);
// float Ray_DistToLineSq(float pX, float pY,
//                      float x1, float y1, float x2, float y2);

void Ray_CastFromCamera(float *distv, Uint32 *colorv, Camera cam, World w);
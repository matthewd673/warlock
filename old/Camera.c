#include <stdlib.h>
#include <math.h>

#include "Camera.h"

struct RayCamera {
    float x;
    float y;
    float angle;
    float projDist;

    float cosAngle;
    float sinAngle;

    float fov;
    int halfRays;
    float *rays;
};

RayCamera new_Camera(float x, float y) {
    RayCamera this = (RayCamera)malloc(sizeof(struct RayCamera));
    if (this == NULL) return NULL;

    this->x = x;
    this->y = y;
    this->angle = 0;
    this->projDist = 300;

    this->cosAngle = 1;
    this->sinAngle = 0;

    this->fov = 3.14 / 2; //3.14 = 180deg
    this->halfRays = 50;
    this->rays = (float *)calloc(this->halfRays * 2, sizeof(float));
    Camera_CalculateRayAngles(this);

    return this;
}

void Camera_Free(RayCamera cam) {
    free(cam->rays);
    free(cam);
}

void Camera_SetPos(RayCamera cam, float x, float y) {
    cam->x = x;
    cam->y = y;
}
void Camera_Move(RayCamera cam, float m) {
    Camera_SetPos(cam, cam->x + cam->cosAngle*m, cam->y + cam->sinAngle*m);
}
float Camera_GetX(RayCamera cam) { return cam->x; }
float Camera_GetY(RayCamera cam) { return cam->y; }

void Camera_SetAngle(RayCamera cam, float angle) {
    cam->angle = angle;
    cam->cosAngle = cos(angle);
    cam->sinAngle = sin(angle);
    Camera_CalculateRayAngles(cam);
}
void Camera_IncAngle(RayCamera cam, float dAngle) {
    Camera_SetAngle(cam, cam->angle + dAngle);
}
float Camera_GetAngle(RayCamera cam) { return cam->angle; }
float Camera_GetCosAngle(RayCamera cam) { return cam->cosAngle; }
float Camera_GetSinAngle(RayCamera cam) { return cam->sinAngle; }

void Camera_SetProjDist(RayCamera cam, float projDist) {
    cam->projDist = projDist;
}
float Camera_GetProjDist(RayCamera cam) { return cam->projDist; }

void Camera_SetFOV(RayCamera cam, float fov) {
    cam->fov = fov;
    Camera_CalculateRayAngles(cam);
}
int Camera_GetFOV(RayCamera cam) { return cam->fov; }

void Camera_SetHalfRays(RayCamera cam, int halfRays) {
    cam->halfRays = halfRays;
    //resize rays array
    free(cam->rays);
    cam->rays = (float *)calloc(halfRays * 2, sizeof(float));
}
int Camera_GetHalfRays(RayCamera cam) { return cam->halfRays; }

void Camera_CalculateRayAngles(RayCamera cam) {
    //https://stackoverflow.com/a/55247059/3785038
    float screenHalfLen = cam->projDist * tan(cam->fov/2.0); //perhaps SCREEN_WIDTH * tan(...)?
    float rayLen = screenHalfLen / cam->halfRays;
    for (int i = 0; i < cam->halfRays * 2; i++) {
        cam->rays[i] = atan(((rayLen*i) - screenHalfLen) / cam->projDist) + cam->angle;
    }

}
float *Camera_GetRayAngles(RayCamera cam) { return cam->rays; }
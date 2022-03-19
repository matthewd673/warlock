#include <stdlib.h>
#include <math.h>

#include "Camera.h"

struct Camera {
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

Camera new_Camera(float x, float y) {
    Camera this = (Camera)malloc(sizeof(struct Camera));
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

void Camera_Free(Camera cam) {
    free(cam->rays);
    free(cam);
}

void Camera_SetPos(Camera cam, float x, float y) {
    cam->x = x;
    cam->y = y;
}
void Camera_Move(Camera cam, float m) {
    Camera_SetPos(cam, cam->x + cam->cosAngle*m, cam->y + cam->sinAngle*m);
}
float Camera_GetX(Camera cam) { return cam->x; }
float Camera_GetY(Camera cam) { return cam->y; }

void Camera_SetAngle(Camera cam, float angle) {
    cam->angle = angle;
    cam->cosAngle = cos(angle);
    cam->sinAngle = sin(angle);
    Camera_CalculateRayAngles(cam);
}
void Camera_IncAngle(Camera cam, float dAngle) {
    Camera_SetAngle(cam, cam->angle + dAngle);
}
float Camera_GetAngle(Camera cam) { return cam->angle; }
float Camera_GetCosAngle(Camera cam) { return cam->cosAngle; }
float Camera_GetSinAngle(Camera cam) { return cam->sinAngle; }

void Camera_SetProjDist(Camera cam, float projDist) {
    cam->projDist = projDist;
}
float Camera_GetProjDist(Camera cam) { return cam->projDist; }

void Camera_SetFOV(Camera cam, float fov) {
    cam->fov = fov;
    Camera_CalculateRayAngles(cam);
}
int Camera_GetFOV(Camera cam) { return cam->fov; }

void Camera_SetHalfRays(Camera cam, int halfRays) {
    cam->halfRays = halfRays;
    //resize rays array
    free(cam->rays);
    cam->rays = (float *)calloc(halfRays * 2, sizeof(float));
}
int Camera_GetHalfRays(Camera cam) { return cam->halfRays; }

void Camera_CalculateRayAngles(Camera cam) {
    // float increment = cam->fov / (cam->halfRays * 2);

    // for (int i = -cam->halfRays; i < cam->halfRays; i++) {
    //     cam->rays[i + cam->halfRays] = cam->angle + increment*i;
    // }

    //https://stackoverflow.com/a/55247059/3785038
    float screenHalfLen = cam->projDist * tan(cam->fov/2.0);
    float rayLen = screenHalfLen / cam->halfRays;
    for (int i = 0; i < cam->halfRays * 2; i++) {
        cam->rays[i] = atan(((rayLen*i) - screenHalfLen) / cam->projDist) + cam->angle;
    }

}
float *Camera_GetRayAngles(Camera cam) { return cam->rays; }
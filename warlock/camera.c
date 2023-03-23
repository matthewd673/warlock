#include <stdlib.h>
#include <math.h>
#include "camera.h"

struct RayCamera {
    float x;
    float y;
    float angle;
    float projDist;

    float cosAngle;
    float sinAngle;

    float fov;
    int halfRayc;
    float *rayv;
};

RayCamera new_Camera(float x, float y, float fov, int rayc, float projDist) {
    RayCamera this = (RayCamera)malloc(sizeof(struct RayCamera));
    if (this == NULL) {
        return NULL;
    }

    this->x = x;
    this->y = y;
    this->angle = 0;
    this->projDist = projDist;

    this->cosAngle = 1;
    this->sinAngle = 0;

    this->fov = fov;
    this->halfRayc = rayc / 2;
    this->rayv = (float *)calloc(rayc, sizeof(float));
    Camera_CalculateRayv(this);

    return this;
}

void free_Camera(RayCamera cam) {
    free(cam->rayv);
    free(cam);
}

void Camera_SetPos(RayCamera cam, float x, float y) {
    cam->x = x;
    cam->y = y;
}

void Camera_MoveForward(RayCamera cam, float m) {
    Camera_SetPos(cam, cam->x + cam->cosAngle*m, cam->y + cam->sinAngle*m);
}

float Camera_GetX(RayCamera cam) {
    return cam->x;
}

float Camera_GetY(RayCamera cam) {
    return cam->y;
}

void Camera_SetAngle(RayCamera cam, float angle) {
    cam->angle = angle;
    cam->cosAngle = cos(angle);
    cam->sinAngle = sin(angle);
    Camera_CalculateRayv(cam);
}

void Camera_Turn(RayCamera cam, float delta) {
    Camera_SetAngle(cam, cam->angle + delta);
}

float Camera_GetAngle(RayCamera cam) {
    return cam->angle;
}

float Camera_GetAngleCos(RayCamera cam) {
    return cam->cosAngle;
}

float Camera_GetAngleSin(RayCamera cam) {
    return cam->sinAngle;
}

float Camera_GetProjDist(RayCamera cam) {
    return cam->projDist;
}

int Camera_GetRayc(RayCamera cam) {
    return cam->halfRayc * 2;
}

int Camera_GetHalfRayc(RayCamera cam) {
    return cam->halfRayc;
}

void Camera_CalculateRayv(RayCamera cam) {
    //https://stackoverflow.com/a/55247059/3785038
    float screenHalfLen = cam->projDist * tan(cam->fov/2.0); //perhaps SCREEN_WIDTH * tan(...)?
    float rayLen = screenHalfLen / cam->halfRayc;
    for (int i = 0; i < cam->halfRayc * 2; i++) {
        cam->rayv[i] = atan(((rayLen*i) - screenHalfLen) / cam->projDist) + cam->angle;
    }
}

float *Camera_GetRayv(RayCamera cam) {
    return cam->rayv;
}
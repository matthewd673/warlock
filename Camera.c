#include <stdlib.h>
#include <math.h>

#include "Camera.h"

struct Camera {
    int x;
    int y;
    float angle;
    float sightMag;

    float cosAngle;
    float sinAngle;

    float fov;
    int halfRays;
    float *rays;
};

Camera new_Camera(int x, int y) {
    Camera this = (Camera)malloc(sizeof(struct Camera));
    if (this == NULL) return NULL;

    this->x = x;
    this->y = y;
    this->angle = 0;
    this->sightMag = 200;
    
    this->cosAngle = 1;
    this->sinAngle = 0;

    this->fov = 3.14 / 2; //90
    this->halfRays = 50;
    this->rays = (float *)calloc(this->halfRays * 2, sizeof(float));
    Camera_CalculateSightRays(this);

    return this;
}

void Camera_Free(Camera cam) {
    free(cam->rays);
    free(cam);
}

void Camera_SetPos(Camera cam, int x, int y) {
    cam->x = x;
    cam->y = y;
}
void Camera_Move(Camera cam, int m) {
    Camera_SetPos(cam, cam->x + cam->cosAngle*m, cam->y + cam->sinAngle*m);
}
int Camera_GetX(Camera cam) { return cam->x; }
int Camera_GetY(Camera cam) { return cam->y; }

void Camera_SetAngle(Camera cam, float angle) {
    cam->angle = angle;
    cam->cosAngle = cos(angle);
    cam->sinAngle = sin(angle);
    Camera_CalculateSightRays(cam);
}
void Camera_IncAngle(Camera cam, float dAngle) {
    Camera_SetAngle(cam, cam->angle + dAngle);
}
float Camera_GetAngle(Camera cam) { return cam->angle; }
float Camera_GetCosAngle(Camera cam) { return cam->cosAngle; }
float Camera_GetSinAngle(Camera cam) { return cam->sinAngle; }

void Camera_SetSightMag(Camera cam, float sightMag) {
    cam->sightMag = sightMag;
}
float Camera_GetSightMag(Camera cam) { return cam->sightMag; }

void Camera_SetFOV(Camera cam, int fov) {
    cam->fov = fov;
    Camera_CalculateSightRays(cam);
}
int Camera_GetFOV(Camera cam) { return cam->fov; }

void Camera_SetHalfRays(Camera cam, int halfRays) {
    cam->halfRays = halfRays;
    //resize rays array
    free(cam->rays);
    cam->rays = (float *)calloc(halfRays * 2, sizeof(float));
}
int Camera_GetHalfRays(Camera cam) { return cam->halfRays; }

void Camera_CalculateSightRays(Camera cam) {
    float increment = cam->fov / (cam->halfRays * 2);

    for (int i = -cam->halfRays; i < cam->halfRays; i++) {
        cam->rays[i + cam->halfRays] = cam->angle + increment*i;
    }
}
float *Camera_GetSightRays(Camera cam) { return cam->rays; }
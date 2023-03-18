typedef struct RayCamera *RayCamera;

RayCamera new_Camera(float x, float y);
void Camera_Free(RayCamera cam);

void Camera_SetPos(RayCamera cam, float x, float y);
void Camera_Move(RayCamera cam, float m);
float Camera_GetX(RayCamera cam);
float Camera_GetY(RayCamera cam);

void Camera_SetAngle(RayCamera cam, float angle);
void Camera_IncAngle(RayCamera cam, float dAngle);
float Camera_GetAngle(RayCamera cam);
float Camera_GetCosAngle(RayCamera cam);
float Camera_GetSinAngle(RayCamera cam);

void Camera_SetProjDist(RayCamera cam, float projDist);
float Camera_GetProjDist(RayCamera cam);

void Camera_SetFOV(RayCamera cam, float fov);
int Camera_GetFOV(RayCamera cam);

void Camera_SetHalfRays(RayCamera cam, int halfRays);
int Camera_GetHalfRays(RayCamera cam);

void Camera_CalculateRayAngles(RayCamera cam);
float *Camera_GetRayAngles(RayCamera cam);
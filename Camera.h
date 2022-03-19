typedef struct Camera *Camera;

Camera new_Camera(float x, float y);
void Camera_Free(Camera cam);

void Camera_SetPos(Camera cam, float x, float y);
void Camera_Move(Camera cam, float m);
float Camera_GetX(Camera cam);
float Camera_GetY(Camera cam);

void Camera_SetAngle(Camera cam, float angle);
void Camera_IncAngle(Camera cam, float dAngle);
float Camera_GetAngle(Camera cam);
float Camera_GetCosAngle(Camera cam);
float Camera_GetSinAngle(Camera cam);

void Camera_SetProjDist(Camera cam, float projDist);
float Camera_GetProjDist(Camera cam);

void Camera_SetFOV(Camera cam, float fov);
int Camera_GetFOV(Camera cam);

void Camera_SetHalfRays(Camera cam, int halfRays);
int Camera_GetHalfRays(Camera cam);

void Camera_CalculateRayAngles(Camera cam);
float *Camera_GetRayAngles(Camera cam);
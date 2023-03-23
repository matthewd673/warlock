typedef struct RayCamera *RayCamera;

RayCamera new_Camera(float x, float y, float fov, int rayc, float projDist);
void free_Camera(RayCamera cam);

void Camera_SetPos(RayCamera cam, float x, float y);
void Camera_MoveForward(RayCamera cam, float m);
float Camera_GetX(RayCamera cam);
float Camera_GetY(RayCamera cam);

void Camera_SetAngle(RayCamera cam, float angle);
void Camera_Turn(RayCamera cam, float delta);
float Camera_GetAngle(RayCamera cam);
float Camera_GetAngleCos(RayCamera cam);
float Camera_GetAngleSin(RayCamera cam);

float Camera_GetProjDist(RayCamera cam);

int Camera_GetRayc(RayCamera cam);
int Camera_GetHalfRayc(RayCamera cam);
void Camera_CalculateRayv(RayCamera cam);
float *Camera_GetRayv(RayCamera cam);
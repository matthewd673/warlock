typedef struct Camera *Camera;

Camera new_Camera();

void Camera_SetPos(Camera cam, int x, int y);
void Camera_Move(Camera cam, int m);
int Camera_GetX(Camera cam);
int Camera_GetY(Camera cam);

void Camera_SetAngle(Camera cam, float angle);
void Camera_IncAngle(Camera cam, float dAngle);
float Camera_GetAngle(Camera cam);
float Camera_GetCosAngle(Camera cam);
float Camera_GetSinAngle(Camera cam);

void Camera_SetSightMag(Camera cam, float sightMag);
float Camera_GetSightMag(Camera cam);

void Camera_SetFOV(Camera cam, int fov);
int Camera_GetFOV(Camera cam);

void Camera_SetHalfRays(Camera cam, int halfRays);
int Camera_GetHalfRays(Camera cam);

void Camera_CalculateSightRays(Camera cam);
float *Camera_GetSightRays(Camera cam);
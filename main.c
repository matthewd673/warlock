#include <stdlib.h>
#include <math.h>

#include "raylib.h"

#include "camera.h"
#include "world.h"
#include "ray.h"

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480

#define FOV                 3.14 / 2
#define CAM_SPEED           4
#define CAM_TURNING_SPEED   0.05

#define WALL_HEIGHT         10

RayCamera cam;
World world;

float *disv;
int *mapv;
int *texv;

void update();
void render();

int main() {
    // create window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib window");
    SetTargetFPS(60);

    // game setup
    cam = new_Camera(0, 0, FOV, SCREEN_WIDTH, SCREEN_HEIGHT);
    world = new_World("world.txt");
    disv = (float *)malloc(Camera_GetRayc(cam) * sizeof(float));
    mapv = (int *)malloc(Camera_GetRayc(cam) * sizeof(int));
    texv = (int *)malloc(Camera_GetRayc(cam) * sizeof(int));

    // game loop
    Ray_Cast(cam, world, disv, mapv, texv);
    while (!WindowShouldClose()) {
        update();
        render();
    }

    // cleanup
    CloseWindow();
    free_Camera(cam);
    free_World(world);

    return 0;
}

void update() {
    // move/turn and recalculate rays
    if (IsKeyDown(KEY_W)) {
        Camera_MoveForward(cam, CAM_SPEED);
        Ray_Cast(cam, world, disv, mapv, texv);
    }
    else if (IsKeyDown(KEY_S)) {
        Camera_MoveForward(cam, -CAM_SPEED);
        Ray_Cast(cam, world, disv, mapv, texv);
    }

    if (IsKeyDown(KEY_A)) {
        Camera_Turn(cam, -CAM_TURNING_SPEED);
        Ray_Cast(cam, world, disv, mapv, texv);
    }
    else if (IsKeyDown(KEY_D)) {
        Camera_Turn(cam, CAM_TURNING_SPEED);
        Ray_Cast(cam, world, disv, mapv, texv);
    }
}

void DrawPerspective() {
    int rayc = Camera_GetRayc(cam);
    float maxDist = Camera_GetProjDist(cam);
    int halfH = SCREEN_HEIGHT / 2;

    for (int i = rayc - 1; i >= 0; i--) {
        float angle = Camera_GetRayv(cam)[i];
        float dist = disv[i];

        // https://stackoverflow.com/a/66664319/3785038
        float wallH = (SCREEN_HEIGHT / dist);
        wallH *= WALL_HEIGHT;
        if (wallH > SCREEN_HEIGHT) {
            wallH = SCREEN_HEIGHT;
        }

        float lightRatio = 1 - (dist / maxDist); // TODO: inverse square stuff

        for (int k = 0; k < round(wallH * 2.0); k++) {
            Color col = WHITE;
            col.r *= lightRatio;
            col.g *= lightRatio;
            col.b *= lightRatio;
            DrawRectangle(i, round(halfH - wallH) + k, 1, 1, col);
        }
    }
}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawPerspective();

    EndDrawing();
}
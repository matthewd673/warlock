#include <stdlib.h>
#include <stdio.h>
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

#define WALL_HEIGHT         12

RayCamera cam;
World world;

float *disv;
int *mapv;
int *texv;

Color *alltexcol;
int alltexw;
int alltexh;

void update();
void render();

int main() {
    // create window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib window");
    SetTargetFPS(60);

    // game setup
    cam = new_Camera(0, 0, FOV, SCREEN_WIDTH, SCREEN_HEIGHT * 2);
    world = new_World("world.txt");
    disv = (float *)malloc(Camera_GetRayc(cam) * sizeof(float));
    mapv = (int *)malloc(Camera_GetRayc(cam) * sizeof(int));
    texv = (int *)malloc(Camera_GetRayc(cam) * sizeof(int));

    Image alltex = LoadImage("stone.png");
    alltexcol = LoadImageColors(alltex);
    alltexw = alltex.width;
    alltexh = alltex.height;

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

void DrawOverhead() {
    // draw walls
    int wallc = World_GetWallc(world);
    Wall *wallv = World_GetWallv(world);
    for (int i = 0; i < wallc; i++) {
        DrawLine(
            Wall_GetX1(wallv[i]), Wall_GetY1(wallv[i]),
            Wall_GetX2(wallv[i]), Wall_GetY2(wallv[i]),
            RAYWHITE);
    }

    // draw camera
    int camx = Camera_GetX(cam);
    int camy = Camera_GetY(cam);
    DrawLine(camx, camy,
        camx + Camera_GetAngleCos(cam) * 10,
        camy + Camera_GetAngleSin(cam) * 10,
        WHITE);
    DrawCircle(camx, camy, 2, WHITE);
}

void DrawPerspective() {
    int rayc = Camera_GetRayc(cam);
    float maxDist = Camera_GetProjDist(cam);
    int halfH = SCREEN_HEIGHT / 2;

    for (int i = rayc - 1; i >= 0; i--) {
        float angle = Camera_GetRayv(cam)[i];
        float dist = disv[i];
        int map = mapv[i];

        // if (dist == maxDist) continue;

        // https://stackoverflow.com/a/66664319/3785038
        float wallH = (SCREEN_HEIGHT / dist) * WALL_HEIGHT;
        wallH = wallH > SCREEN_HEIGHT ? SCREEN_HEIGHT : wallH;

        float lightRatio = 1 - (dist / maxDist); // TODO: inverse square stuff

        float wallRenderH = round(wallH * 2.0);
        float texHRatio = alltexh / (wallH * 2.0);
        for (int k = 0; k < SCREEN_HEIGHT; k++) {
            // render walls when in range
            if (dist != maxDist &&
                k >= round(halfH-wallH) &&
                k <= round(halfH-wallH) + wallRenderH) {
                // texture point row and column
                int texr = (int)floor((k-round(halfH-wallH)) * texHRatio);
                int texc = map % alltexw;

                Color col = alltexcol[texr * alltexw + texc];

                col.r *= lightRatio;
                col.g *= lightRatio;
                col.b *= lightRatio;

                DrawRectangle(i, k, 1, 1, col);
            }
            // floor
            else if (k >= halfH) {
                float ray = Camera_GetRayv(cam)[i];
                float beta = fabsf(ray - Camera_GetAngle(cam));
                int r = k - halfH;
                int playerH = WALL_HEIGHT / 2;
                float straightLineDist = playerH * Camera_GetProjDist(cam) / r;

                float d = straightLineDist / cos(beta);

                float fx = Camera_GetX(cam) + cos(ray) * d;
                float fy = Camera_GetY(cam) + sin(ray) * d;

                int texr = abs((int)round(fy) % alltexh);
                int texc = abs((int)round(fx) % alltexw);
                Color col = alltexcol[texr * alltexw + texc];
                // col.r += fy;
                // col.g += fx;
                // col.b += fy;
                // col.r %= 255;
                // col.g %= 255;
                // col.b %= 255;

                DrawRectangle(i, k, 1, 1, col);
            }
            // ceiling
            else if (k <= halfH) {
                DrawRectangle(i, k, 1, 1, DARKBLUE);
            }
        }
    }
}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawPerspective();
    DrawOverhead();

    EndDrawing();
}
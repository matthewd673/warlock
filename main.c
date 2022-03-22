#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "Camera.h"
#include "Render.h"
#include "Ray.h"
#include "Texture.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FOV 90
#define CAM_SPEED 2
#define CAM_TURNING_SPEED 0.1

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;

SDL_Surface *gCanvas = NULL;

int Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize video: %s\n", SDL_GetError());
        return 0;
    }

    gWindow = SDL_CreateWindow(
        "sdlray",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (gWindow == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return 0;
    }
    
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return 1;
}

int CreateSurface() {
    gCanvas = SDL_CreateRGBSurface(
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        32,
        0xff000000,
        0x00ff0000,
        0x0000ff00,
        0x000000ff);
    
    if (gCanvas == NULL) return 0;

    return 1;
}

void Quit() {
    SDL_FreeSurface(gCanvas);
    gCanvas = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char *argv[]) {
    if (!Init()) {
        printf("Failed to initialize\n");
        return;
    }

    if (!CreateSurface()) {
        printf("Failed to load media\n");
        return;
    }

    // Texture stone = new_Texture("content/stone.png");

    int abort = 0;
    SDL_Event e;

    Camera cam = new_Camera(0, 0);
    Camera_SetHalfRays(cam, SCREEN_WIDTH / 2);
    Camera_SetFOV(cam, (FOV/360.0) * 2*3.14);
    Camera_SetProjDist(cam, SCREEN_HEIGHT);
    World world = new_World("input.txt", gCanvas->format);

    float *distv = (float *)calloc(Camera_GetHalfRays(cam) * 2, sizeof(float));
    int *mapv = (int *)malloc(Camera_GetHalfRays(cam) * 2 * sizeof(int));
    int *texv = (int *)malloc(Camera_GetHalfRays(cam) * 2 * sizeof(int));
    Ray_CastFromCamera(distv, mapv, texv, cam, world, gCanvas->format);

    //game loop
    while (!abort) {
        //event loop
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) abort = 1;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        abort = 1;
                        break;
                    case SDLK_w:
                        Camera_Move(cam, CAM_SPEED);
                        Ray_CastFromCamera(distv, mapv, texv, cam, world, gCanvas->format);
                        break;
                    case SDLK_a:
                        Camera_IncAngle(cam, -CAM_TURNING_SPEED);
                        Ray_CastFromCamera(distv, mapv, texv, cam, world, gCanvas->format);
                        break;
                    case SDLK_s:
                        Camera_Move(cam, -CAM_SPEED);
                        Ray_CastFromCamera(distv, mapv, texv, cam, world, gCanvas->format);
                        break;
                    case SDLK_d:
                        Camera_IncAngle(cam, CAM_TURNING_SPEED);
                        Ray_CastFromCamera(distv, mapv, texv, cam, world, gCanvas->format);
                        break;
                }
            }
        }

        //BEGIN RENDER
        //TODO: this is sloppy, but good enough for now
        SDL_FillRect(gScreenSurface, NULL, 0x000000);
        SDL_FillRect(gCanvas, NULL, 0x000000);

        //RENDER
        DrawPerspective(gCanvas, cam, world, distv, mapv, texv, SCREEN_HEIGHT);

        // DrawRays(gCanvas, cam, distv);
        // DrawCamera(gCanvas, cam);
        // DrawWorld(gCanvas, world);

        //END RENDER
        SDL_BlitSurface(gCanvas, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
    }

    //free
    free(distv);
    // free(colorv);
    Camera_Free(cam);
    World_Free(world);

    Quit();
}
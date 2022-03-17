#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "Camera.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

void SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint32 *const target = (Uint32 *) ((Uint8 *) surface->pixels
                                                    + y * surface->pitch
                                                    + x * surface->format->BytesPerPixel);
    *target = SDL_MapRGB(surface->format, r, g, b);
}

//Bresenham's algorithm
//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void DrawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;

    while (1) {
        SetPixel(surface, x1, y1, r, g, b);
        if (x1 == x2 && y1 == y2) break;
        int twoError = error * 2;
        if (twoError >= dy) {
            if (x1 == x2) break;
            error += dy;
            x1 += sx;
        }
        if (twoError <= dx) {
            if (y1 == y2) break;
            error += dx;
            y1 += sy;
        }
    }
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

    int abort = 0;
    SDL_Event e;

    Camera cam = new_Camera();

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
                        Camera_Move(cam, 5);
                        break;
                    case SDLK_a:
                        Camera_IncAngle(cam, -0.05);
                        break;
                    case SDLK_s:
                        Camera_Move(cam, -5);
                        break;
                    case SDLK_d:
                        Camera_IncAngle(cam, 0.05);
                        break;
                }
            }
        }

        //BEGIN RENDER
        //TODO: this is sloppy, but good enough for now
        SDL_FillRect(gScreenSurface, NULL, 0x000000);
        SDL_FillRect(gCanvas, NULL, 0x000000);

        //RENDER
        //visualize rays
        for (int i = 0; i < Camera_GetHalfRays(cam) * 2; i++) {
            DrawLine(
                gCanvas,
                Camera_GetX(cam),
                Camera_GetY(cam),
                Camera_GetX(cam) + cos(Camera_GetSightRays(cam)[i])*Camera_GetSightMag(cam),
                Camera_GetY(cam) + sin(Camera_GetSightRays(cam)[i])*Camera_GetSightMag(cam),
                100, 100, 100
            );
        }

        //visualize camera
        DrawLine(
            gCanvas,
            Camera_GetX(cam),
            Camera_GetY(cam),
            Camera_GetX(cam) + Camera_GetCosAngle(cam)*Camera_GetSightMag(cam),
            Camera_GetY(cam) + Camera_GetSinAngle(cam)*Camera_GetSightMag(cam),
            255, 255, 255
            );

        //END RENDER
        SDL_BlitSurface(gCanvas, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
    }

    Quit();
}
#include "Render.h"
#include <math.h>

#define TEXTURE_HEIGHT 64

void SetPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint32 *const target = (Uint32 *) ((Uint8 *) surface->pixels
                                                    + y * surface->pitch
                                                    + x * surface->format->BytesPerPixel);
    *target = color;
}

void SetPixelRGB(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    SetPixel(surface, x, y, SDL_MapRGB(surface->format, r, g, b));
}

//Bresenham's algorithm
//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void DrawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;

    while (1) {
        SetPixel(surface, x1, y1, color);
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

void DrawLineRGB(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    DrawLine(surface, x1, y1, x2, y2, SDL_MapRGB(surface->format, r, g, b));
}

void DrawCamera(SDL_Surface *surface, Camera cam) {
    DrawLineRGB(
        surface,
        Camera_GetX(cam),
        Camera_GetY(cam),
        Camera_GetX(cam) + Camera_GetCosAngle(cam)*Camera_GetSightMag(cam),
        Camera_GetY(cam) + Camera_GetSinAngle(cam)*Camera_GetSightMag(cam),
        255, 255, 255
        );
}

void DrawRays(SDL_Surface *surface, Camera cam, float *distv) {
    int distc = Camera_GetHalfRays(cam) * 2;
    for (int i = 0; i < distc; i++) {
        float angle = Camera_GetSightRays(cam)[i];
        float len = distv[i];

        DrawLineRGB(
            surface,
            Camera_GetX(cam),
            Camera_GetY(cam),
            Camera_GetX(cam) + cos(angle)*len,
            Camera_GetY(cam) + sin(angle)*len,
            100, 100, 100
        );
    }
}

void DrawWall(SDL_Surface *surface, Wall w) {
    //always draw purple
    DrawLineRGB(surface, Wall_GetX1(w), Wall_GetY1(w), Wall_GetX2(w), Wall_GetY2(w), 159, 111, 222);
}

void DrawWorld(SDL_Surface *surface, World w) {
    int ct = World_GetWallCt(w);
    for (int i = 0; i < ct; i++) {
        DrawWall(surface, World_GetWalls(w)[i]);
    }
}

void DrawPerspective(SDL_Surface *surface, Camera cam, World world, float *distv, int *mapv, int *texv, int screenH) {
    int distc = Camera_GetHalfRays(cam) * 2;
    float maxDist = Camera_GetSightMag(cam);
    int halfH = screenH / 2;

    Uint32 *colv = (Uint32 *)malloc(TEXTURE_HEIGHT * sizeof(Uint32));
    for (int i = distc - 1; i >= 0; i--) { //TODO: i feel like something isn't right here
        float dist = distv[i];
        if (dist == Camera_GetSightMag(cam)) continue;

        float ratio = 1 - dist/maxDist;
        int drawH = halfH * ratio;

        Texture_GetColumn(surface, World_GetTextures(world)[texv[i]], mapv[i], colv);
        float texHRatio = TEXTURE_HEIGHT / (float)(drawH * 2);
        for (int k = 0; k < drawH * 2; k++) {
            SetPixel(surface, i, halfH - drawH + k, colv[(int)round(k * texHRatio)]);
        }
    }

}
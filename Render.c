#include "Render.h"

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

void DrawCamera(SDL_Surface *surface, Camera cam) {
    //visualize rays
    for (int i = 0; i < Camera_GetHalfRays(cam) * 2; i++) {
        DrawLine(
            surface,
            Camera_GetX(cam),
            Camera_GetY(cam),
            Camera_GetX(cam) + cos(Camera_GetSightRays(cam)[i])*Camera_GetSightMag(cam),
            Camera_GetY(cam) + sin(Camera_GetSightRays(cam)[i])*Camera_GetSightMag(cam),
            100, 100, 100
        );
    }

    //visualize camera
    DrawLine(
        surface,
        Camera_GetX(cam),
        Camera_GetY(cam),
        Camera_GetX(cam) + Camera_GetCosAngle(cam)*Camera_GetSightMag(cam),
        Camera_GetY(cam) + Camera_GetSinAngle(cam)*Camera_GetSightMag(cam),
        255, 255, 255
        );
}

void DrawWall(SDL_Surface *surface, Wall w) {
    DrawLine(surface, Wall_GetX1(w), Wall_GetY1(w), Wall_GetX2(w), Wall_GetY2(w), Wall_GetR(w), Wall_GetG(w), Wall_GetB(w));
}

void DrawWorld(SDL_Surface *surface, World w) {
    int ct = World_GetWallCt(w);
    for (int i = 0; i < ct; i++) {
        DrawWall(surface, World_GetWalls(w)[i]);
    }
}
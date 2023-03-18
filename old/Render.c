#include "Render.h"
#include <math.h>

#define TEXTURE_HEIGHT 64
#define TEXTURE_WIDTH 64
#define TEXTURE_HOR_SCALE 1
#define WALL_HEIGHT 10

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

void DrawCamera(SDL_Surface *surface, RayCamera cam) {
    DrawLineRGB(
        surface,
        Camera_GetX(cam),
        Camera_GetY(cam),
        Camera_GetX(cam) + Camera_GetCosAngle(cam)*Camera_GetProjDist(cam),
        Camera_GetY(cam) + Camera_GetSinAngle(cam)*Camera_GetProjDist(cam),
        255, 255, 255
        );
}

void DrawRays(SDL_Surface *surface, RayCamera cam, float *distv) {
    int distc = Camera_GetHalfRays(cam) * 2;
    for (int i = 0; i < distc; i++) {
        float angle = Camera_GetRayAngles(cam)[i];
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

void DrawPerspective(SDL_Surface *surface, RayCamera cam, World world, float *distv, int *mapv, int *texv, int screenH) {
    int distc = Camera_GetHalfRays(cam) * 2;
    float maxDist = Camera_GetProjDist(cam);
    int halfH = screenH / 2;

    Uint32 *colv = (Uint32 *)malloc(TEXTURE_HEIGHT * sizeof(Uint32));
    for (int i = distc - 1; i >= 0; i--) {

        float angle = Camera_GetRayAngles(cam)[i];

        //https://gamedev.stackexchange.com/q/159285
        for (int k = halfH; k < screenH; k++) {
            float floorDist = (WALL_HEIGHT/(float)(k-halfH)) * Camera_GetProjDist(cam);// * cos(angle - Camera_GetAngle(cam));

            // floorDist *= cos(angle - Camera_GetAngle(cam));

            float floorX = floorDist * cos(angle) + Camera_GetX(cam);
            float floorY = floorDist * sin(angle) + Camera_GetY(cam);

            int floorDrawX = (int)floorX % TEXTURE_WIDTH;
            int floorDrawY = (int)floorY % TEXTURE_HEIGHT;

            float floorLightRatio = 1 - (floorDist / maxDist);

            Uint32 color = Texture_GetPixel(surface, World_GetFloorTexture(world), floorDrawX, floorDrawY);
            SetPixelRGB(surface,
                i, k,
                ((color >> 24) & 0x000000ff) * floorLightRatio,
                ((color >> 16) & 0x000000ff) * floorLightRatio,
                ((color >> 8) & 0x000000ff) * floorLightRatio
                );
        }

        float dist = distv[i];
        // if (dist == maxDist) continue;

        //https://stackoverflow.com/a/66664319/3785038
        float wallH = (screenH / dist);
        wallH *= WALL_HEIGHT;
        if (wallH > screenH) wallH = screenH;

        float lightRatio = 1 - (dist / maxDist);

        Texture_GetColumn(surface, World_GetTextures(world)[texv[i]], (int)round(mapv[i] * TEXTURE_HOR_SCALE) % TEXTURE_WIDTH, colv);
        float texHRatio = TEXTURE_HEIGHT / (wallH * 2);
        for (int k = 0; k < round(wallH * 2.0); k++) {
            Uint32 color = colv[(int)round(k * texHRatio)];
            SetPixelRGB(surface,
                    i,
                    round(halfH - wallH) + k,
                    ((color >> 24) & 0x000000ff) * lightRatio,
                    ((color >> 16) & 0x000000ff) * lightRatio,
                    ((color >> 8) & 0x000000ff) * lightRatio
                    );
        }

    }

}
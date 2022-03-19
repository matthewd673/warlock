#include <SDL2/SDL.h>

#include "Camera.h"
#include "World.h"
#include "Texture.h"

void SetPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void SetPixelRGB(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void DrawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
void DrawLineRGB(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void DrawCamera(SDL_Surface *surface, Camera cam);
void DrawRays(SDL_Surface *surface, Camera cam, float *distv);
void DrawWall(SDL_Surface *surface, Wall w);
void DrawWorld(SDL_Surface *surface, World w);
void DrawPerspective(SDL_Surface *surface, Camera cam, World world, float *distv, int *mapv, int *texv, int screenH);
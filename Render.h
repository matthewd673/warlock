#include <SDL2/SDL.h>

#include "Camera.h"
#include "World.h"

void SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void DrawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void DrawCamera(SDL_Surface *surface, Camera cam);
void DrawWall(SDL_Surface *surface, Wall w);
void DrawWorld(SDL_Surface *surface, World w);
void DrawPerspective(SDL_Surface *surface, Camera cam, float *distv, int SCREEN_HEIGHT);
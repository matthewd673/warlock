#include <SDL2/SDL.h>
#include "Texture.h"

typedef struct Wall *Wall;
typedef struct World *World;

Wall new_Wall(int x1, int y1, int x2, int y2, int textureId);

int Wall_GetX1(Wall w);
int Wall_GetY1(Wall w);
int Wall_GetX2(Wall w);
int Wall_GetY2(Wall w);

int Wall_GetTextureId(Wall w);

World new_World(char const *filepath, SDL_PixelFormat *format);
void World_Free(World w);

Wall *World_GetWalls(World w);
int World_GetWallCt(World w);

Texture *World_GetTextures(World w);
int World_GetTextureCt(World w);

Texture World_GetFloorTexture(World w);
Texture World_GetCeilTexture(World w);
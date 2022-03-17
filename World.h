#include <SDL2/SDL.h>

typedef struct Wall *Wall;
typedef struct World *World;

Wall new_Wall(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

int Wall_GetX1(Wall w);
int Wall_GetY1(Wall w);
int Wall_GetX2(Wall w);
int Wall_GetY2(Wall w);

Uint8 Wall_GetR(Wall w);
Uint8 Wall_GetG(Wall w);
Uint8 Wall_GetB(Wall w);

World new_World(char const *filepath);
Wall *World_GetWalls(World w);
int World_GetWallCt(World w);
void World_Free(World w);
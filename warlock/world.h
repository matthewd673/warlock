#include "raylib.h"

typedef struct WTexture *WTexture;
typedef struct Wall *Wall;
typedef struct WSprite *WSprite;

typedef struct World *World;

WTexture new_WTexture(Color *col, int w, int h);
void free_WTexture(WTexture t);
Color *WTexture_GetColors(WTexture t);
int WTexture_GetWidth(WTexture t);
int WTexture_GetHeight(WTexture t);

Wall new_Wall(int x1, int y1, int x2, int y2, int texi);
int Wall_GetX1(Wall w);
int Wall_GetY1(Wall w);
int Wall_GetX2(Wall w);
int Wall_GetY2(Wall w);
int Wall_GetTexi(Wall w);

WSprite new_WSprite()

World new_World(char const *filepath);
void free_World(World w);

int World_GetWallc(World w);
Wall *World_GetWallv(World w);

int World_GetTexc(World w);
WTexture *World_GetTexv(World w);

int World_GetFloorTexi(World w);
int World_GetCeilTexi(World w);
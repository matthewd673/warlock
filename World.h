typedef struct Wall *Wall;
typedef struct World *World;

Wall new_Wall(int x1, int y1, int x2, int y2);
int Wall_GetX1(Wall w);
int Wall_GetY1(Wall w);
int Wall_GetX2(Wall w);
int Wall_GetY2(Wall w);

World new_World(char const *filepath);
void free_World(World w);

Wall *World_GetWallv(World w);
int World_GetWallc(World w);
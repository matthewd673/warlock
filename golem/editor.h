typedef struct Wall *Wall;
typedef struct Workspace *Workspace;

Wall new_Wall(int x1, int y1, int x2, int y2);
int Wall_getX1(Wall w);
int Wall_getY1(Wall w);
int Wall_getX2(Wall w);
int Wall_getY2(Wall w);

Workspace new_Workspace();
LL Workspace_getWalls(Workspace w);
LL Workspace_getSprites(Workspace w);
#include <stdlib.h>

#include "ll.h"
#include "editor.h"

struct Wall {
    int x1;
    int y1;
    int x2;
    int y2;
};

Wall new_Wall(int x1, int y1, int x2, int y2) {
    Wall this = (Wall)malloc(sizeof(struct Wall));

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    return this;
}

int Wall_getX1(Wall w) {
    return w->x1;
}

int Wall_getY1(Wall w) {
    return w->y1;
}

int Wall_getX2(Wall w) {
    return w->x2;
}

int Wall_getY2(Wall w) {
    return w->y2;
}

struct Workspace {
    LL wallv;
    LL spritev;
};

Workspace new_Workspace() {
    Workspace this = (Workspace)malloc(sizeof(struct Workspace));

    this->wallv = new_LL();
    this->spritev = new_LL();

    return this;
}

LL Workspace_getWalls(Workspace w) {
    return w->wallv;
}

LL Workspace_getSprites(Workspace w) {
    return w->spritev;
}
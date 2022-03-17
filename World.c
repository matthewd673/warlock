#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "World.h"

struct Wall {
    int x1;
    int y1;
    int x2;
    int y2;
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

Wall new_Wall(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    Wall this = (Wall)malloc(sizeof(struct Wall));
    if (this == NULL) return NULL;

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    //purple by default, why not
    this->r = r;
    this->g = g;
    this->b = b;

    return this;
}

int Wall_GetX1(Wall w) { return w->x1; }
int Wall_GetY1(Wall w) { return w->y1; }
int Wall_GetX2(Wall w) { return w->x2; }
int Wall_GetY2(Wall w) { return w->y2; }

Uint8 Wall_GetR(Wall w) { return w->r; }
Uint8 Wall_GetG(Wall w) { return w->g; }
Uint8 Wall_GetB(Wall w) { return w->b; }

struct World {
    Wall *walls;
    int wallCt;
};

World new_World(char const *filepath) {
    World this = (World)malloc(sizeof(struct World));
    if (this == NULL) return NULL;

    this->walls = (Wall *)malloc(512 * sizeof(struct Wall));
    int wallIndex = 0;

    FILE *f;
    char buffer[4096];
    f = fopen(filepath, "r");

    Uint8 currentR = 255;
    Uint8 currentG = 255;
    Uint8 currentB = 255;

    fgets(buffer, 4096, f);
    while (fgets(buffer, 4096, f) != NULL) {
        char *token = strtok(buffer, " ");
        
        int *data;
        int dataType = -1;

        int i = 0;
        while (token != NULL) {
            printf("%s\n", token);

            if (i == 0) {
                if (token[0] == '#') break; //comment
                if (token[0] == 'E') {
                    dataType = 0;
                    data = (int *)calloc(4, sizeof(int));
                }
                if (token[0] == 'C') {
                    dataType = 1;
                    data = (int *)calloc(3, sizeof(int));
                }
            }
            else {
                printf("writing data to %d\n", i - 1);
                data[i - 1] = atoi(token);
            }

            token = strtok(NULL, " ");
            i++;
        }

        if (dataType == 0) { //make a new wall
            printf("rgb: %d %d %d\n", currentR, currentG, currentB);
            Wall w = new_Wall(data[0], data[1], data[2], data[3], currentR, currentG, currentB);
            this->walls[wallIndex] = w;
            wallIndex++;
        }
        else if (dataType == 1) { //update color info
            currentR = (Uint8)data[0];
            currentG = (Uint8)data[1];
            currentB = (Uint8)data[2];
        }

        free(data);

    }

    fclose(f);
    free(f);

    this->wallCt = wallIndex;
    printf("ct: %d\n", this->wallCt);
    return this;
}

Wall *World_GetWalls(World w) { return w->walls; }

int World_GetWallCt(World w) { return w->wallCt; }

void World_Free(World w) {
    // free(w->walls);
    for (int i = 0; i < w->wallCt; i++) {
        free(w->walls[i]);
    }
    free(w->walls);
    free(w);
}
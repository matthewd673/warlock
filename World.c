#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "world.h"

struct Wall {
    int x1;
    int y1;
    int x2;
    int y2;
};

Wall new_Wall(int x1, int y1, int x2, int y2) {
    Wall this = (Wall)malloc(sizeof(struct Wall));
    if (this == NULL) {
        return NULL;
    }

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    return this;
}

int Wall_GetX1(Wall w) {
    return w->x1;
}

int Wall_GetY1(Wall w) {
    return w->y1;
}

int Wall_GetX2(Wall w) {
    return w->x2;
}

int Wall_GetY2(Wall w) {
    return w->y2;
}

struct World {
    int wallc;
    Wall *wallv;
};

World new_World(char const *filepath) {
    World this = (World)malloc(sizeof(struct World));
    if (this == NULL) {
        return NULL;
    }

    this->wallv = (Wall *)malloc(512 * sizeof(struct Wall)); // arbitrary count
    int wallIndex = 0;

    FILE *f;
    char buffer[4096]; // arbitrary buffer
    f = fopen(filepath, "r");

    // parse file
    fgets(buffer, 4096, f);
    while (fgets(buffer, 4096, f) != NULL) {
        char *token = strtok(buffer, " ");

        // comment
        if (token[0] == '#') {
            continue;
        }
        // edge
        else if (token[0] == 'e' || token[0] == 'E') {
            // consume 4 ints
            Wall w = new_Wall(
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " "))
            );
            this->wallv[wallIndex] = w;
            wallIndex++;
        }
    }

    fclose(f);

    this->wallc = wallIndex;
    printf("%d wallv loaded from %s", this->wallc, filepath);
    return this;
}

void free_World(World w) {
    for (int i = 0; i < w->wallc; i++) {
        free(w->wallv[i]);
    }
    free(w->wallv);
    free(w);
}

Wall *World_GetWallv(World w) {
    return w->wallv;
}

int World_GetWallc(World w) {
    return w->wallc;
}
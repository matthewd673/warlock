#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "world.h"
#include "raylib.h"

struct WTexture {
    Color *col;
    int w;
    int h;
};

WTexture new_WTexture(Color *col, int w, int h) {
    WTexture this = (WTexture)malloc(sizeof(struct WTexture));
    if (this == NULL) {
        return NULL;
    }

    this->col = col;
    this->w = w;
    this->h = h;

    return this;
}

Color *WTexture_GetColors(WTexture t) {
    return t->col;
}

int WTexture_GetWidth(WTexture t) {
    return t->w;
}

int WTexture_GetHeight(WTexture t) {
    return t->h;
}

struct Wall {
    int x1;
    int y1;
    int x2;
    int y2;
    int texi;
};

Wall new_Wall(int x1, int y1, int x2, int y2, int texi) {
    Wall this = (Wall)malloc(sizeof(struct Wall));
    if (this == NULL) {
        return NULL;
    }

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->texi = texi;

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

int Wall_GetTexi(Wall w) {
    return w->texi;
}

struct World {
    int wallc;
    Wall *wallv;
    int texc;
    WTexture *texv;
    int floortexi;
    int ceiltexi;
};

World new_World(char const *filepath) {
    World this = (World)malloc(sizeof(struct World));
    if (this == NULL) {
        return NULL;
    }

    this->wallv = (Wall *)malloc(512 * sizeof(struct Wall)); // arbitrary count
    if (this->wallv == NULL) {
        return NULL;
    }
    int wallIndex = 0;

    int texIndex = 0;
    int currentTexIndex = 0;

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
        // allocate textures array
        else if (token[0] == 'a' || token[0] == 'A') {
            // consume 1 int (length)
            this->texc = atoi(strtok(NULL, " "));
            this->texv = (WTexture *)malloc(this->texc * sizeof(WTexture));
            if (this->texv == NULL) {
                return NULL;
            }
        }
        // load texture
        else if (token[0] == 'l' || token[0] == 'L') {
            // consume string (filepath)
            Image img = LoadImage(strtok(NULL, "\n"));
            this->texv[texIndex] = new_WTexture(
                LoadImageColors(img),
                img.width,
                img.height
                );
            texIndex++;
        }
        // assign current texture
        else if (token[0] == 't' || token[0] == 'T') {
            currentTexIndex = atoi(strtok(NULL, " "));
        }
        // assign floor texture
        else if (token[0] == 'f' || token[0] == 'F') {
            this->floortexi = atoi(strtok(NULL, " "));
        }
        // assign ceiling texture
        else if (token[0] == 'c' || token[0] == 'C') {
            this->ceiltexi = atoi(strtok(NULL, " "));
        }
        // edge (wall)
        else if (token[0] == 'e' || token[0] == 'E') {
            // consume 4 ints (x1, y1, x2, y2)
            Wall w = new_Wall(
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                currentTexIndex
            );
            this->wallv[wallIndex] = w;
            wallIndex++;
        }
    }

    fclose(f);

    this->wallc = wallIndex;
    printf("%d walls loaded from %s\n", this->wallc, filepath);
    return this;
}

void free_World(World w) {
    for (int i = 0; i < w->wallc; i++) {
        free(w->wallv[i]);
    }
    free(w->wallv);

    for (int i = 0; i < w->texc; i++) {
        free(w->texv[i]);
    }
    free(w->texv);

    free(w);
}

int World_GetWallc(World w) {
    return w->wallc;
}

Wall *World_GetWallv(World w) {
    return w->wallv;
}

int World_GetTexc(World w) {
    return w->texc;
}

WTexture *World_GetTexv(World w) {
    return w->texv;
}

int World_GetFloorTexi(World w) {
    return w->floortexi;
}

int World_GetCeilTexi(World w) {
    return w->ceiltexi;
}
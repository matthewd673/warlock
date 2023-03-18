#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "World.h"
#include "Texture.h"

struct Wall {
    int x1;
    int y1;
    int x2;
    int y2;
    int textureId;
};

Wall new_Wall(int x1, int y1, int x2, int y2, int textureId) {
    Wall this = (Wall)malloc(sizeof(struct Wall));
    if (this == NULL) return NULL;

    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    this->textureId = textureId;
    return this;
}

int Wall_GetX1(Wall w) { return w->x1; }
int Wall_GetY1(Wall w) { return w->y1; }
int Wall_GetX2(Wall w) { return w->x2; }
int Wall_GetY2(Wall w) { return w->y2; }

int Wall_GetTextureId(Wall w) { return w->textureId; }

struct World {
    int wallCt;
    int textureCt;
    int floorTexture;
    int ceilTexture;
    Wall *walls;
    Texture *textures;
};

World new_World(char const *filepath, SDL_PixelFormat *format) {
    World this = (World)malloc(sizeof(struct World));
    if (this == NULL) return NULL;

    this->walls = (Wall *)malloc(512 * sizeof(struct Wall)); //hardcoded cap of 512 walls
    int wallIndex = 0;

    FILE *f;
    char buffer[4096];
    f = fopen(filepath, "r");

    int textureLoadI = 0;
    int currentTexture = 1;

    fgets(buffer, 4096, f);
    while (fgets(buffer, 4096, f) != NULL) {        
        char *token = strtok(buffer, " ");
        
        if (token[0] == '#') continue;
        else if (token[0] == 'E') { //new edge
            //expects 4 int tokens
            Wall w = new_Wall(
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                atoi(strtok(NULL, " ")),
                currentTexture
            );
            this->walls[wallIndex] = w;
            wallIndex++;
        }
        else if (token[0] == 'L') { //load texture
            //expects 1 string token
            this->textures[textureLoadI] = new_Texture(
                strtok(NULL, "\n")
            );
            textureLoadI++;
        }
        else if (token[0] == 'A') { //allocate texture array
            //expects 1 int token
            this->textureCt = atoi(strtok(NULL, " "));
            this->textures = new_TextureArray(this->textureCt);
        }
        else if (token[0] == 'T') { //set current texture i
            currentTexture = atoi(strtok(NULL, " ")) - 1; //1-indexed
        }
        else if (token[0] == 'F') { //set floor texture i
            this->floorTexture = atoi(strtok(NULL, " ")) - 1;
        }
        else if (token[0] == 'C') { //set ceil texture i
            this->ceilTexture = atoi(strtok(NULL, " ")) - 1;
        }
    }

    fclose(f);

    this->wallCt = wallIndex;
    printf("%d wall(s) loaded from '%s'\n", this->wallCt, filepath);
    return this;
}

void World_Free(World w) {
    // free(w->walls);
    for (int i = 0; i < w->wallCt; i++) {
        free(w->walls[i]);
    }
    free(w->walls);
    free(w);
}

Wall *World_GetWalls(World w) { return w->walls; }
int World_GetWallCt(World w) { return w->wallCt; }

Texture *World_GetTextures(World w) { return w->textures; }
int World_GetTextureCt(World w) { return w->textureCt; }

Texture World_GetFloorTexture(World w) { return w->textures[w->floorTexture]; }
Texture World_GetCeilTexture(World w) { return w->textures[w->ceilTexture]; }
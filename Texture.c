#include "Texture.h"
#include "Render.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

struct Texture {
    int width;
    int height;
    int channels;
    int size;
    unsigned char *img;
};

Texture new_Texture(char const *filepath) {
    int width, height, channels;
    unsigned char *img = stbi_load(filepath, &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error loading image '%s'\n", filepath);
    }
    printf("Loaded image (w: %d, h: %d, c: %d)\n", width, height, channels);

    Texture this = (Texture)malloc(sizeof(struct Texture));
    if (this == NULL) return NULL;

    this->width = width;
    this->height = height;
    this->channels = channels;
    this->size = width * height * channels;
    this->img = img;

    return this;
}

void Texture_Free(Texture texture) {
    //TODO
}

int Texture_GetWidth(Texture texture) { return texture->width; }
int Texture_GetHeight(Texture texture) { return texture->height; }
int Texture_GetChannels(Texture texture) { return texture->channels; }
int Texture_GetSize(Texture texture) { return texture->size; }
unsigned char *Texture_GetRawImg(Texture texture) { return texture->img; }

//really just for debug purposes to make sure its loaded
void Texture_DrawImage(SDL_Surface *surface, Texture texture) {
    for (int i = 0; i < texture->width; i++) {
        for (int j = 0; j < texture->height; j++) {
            SetPixel(surface, i, j, Texture_GetPixel(surface, texture, i, j));
        }
    }
}

void Texture_GetColumn(SDL_Surface *surface, Texture texture, int colI, Uint32 *colv) {
    int i = 0;
    for (unsigned char *p = texture->img + colI*texture->channels;
         i < texture->height;
         p += texture->width*texture->channels) {
        colv[i] = SDL_MapRGB(surface->format, *p, *(p + 1), *(p + 2));
        i++;
    }
}

Uint32 Texture_GetPixel(SDL_Surface *surface, Texture texture, int x, int y) {
    unsigned char *p = texture->img + (texture->width*y + x)*texture->channels;
    return SDL_MapRGB(surface->format, *p, *(p + 1), *(p + 2));
}

Texture *new_TextureArray(int ct) {
    Texture *this = (Texture *)malloc(ct * sizeof(Texture));
    if (this == NULL) return NULL;

    return this;
}
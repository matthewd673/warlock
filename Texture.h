typedef struct Texture *Texture;

#include <SDL2/SDL.h>

typedef struct Texture *Texture;

Texture new_Texture(char const *filename);
void Texture_Free(Texture texture);

int Texture_GetWidth(Texture texture);
int Texture_GetHeight(Texture texture);
int Texture_GetChannels(Texture texture);
int Texture_GetSize(Texture texture);
unsigned char *Texture_GetRawImg(Texture texture);

void Texture_DrawImage(SDL_Surface *surface, Texture texture);
void Texture_GetColumn(SDL_Surface *surface, Texture texture, int colI, Uint32 *colv);

Texture *new_TextureArray(int ct);
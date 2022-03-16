#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct Vec2 {
    int x;
    int y;
} *Vec2;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;

SDL_Surface *gCanvas = NULL;

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize video: %s\n", SDL_GetError());
        return 0;
    }

    gWindow = SDL_CreateWindow(
        "sdlray",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (gWindow == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return 0;
    }
    
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return 1;
}

SDL_Surface *loadSurface(char *path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (surface == NULL) {
        printf("Failed to load image: %s\n", SDL_GetError());
    }

    return surface;
}

int createSurface() {
    gCanvas = SDL_CreateRGBSurface(
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        32,
        0xff000000,
        0x00ff0000,
        0x0000ff00,
        0x000000ff);
    
    if (gCanvas == NULL) return 0;

    return 1;
}

void quit() {
    SDL_FreeSurface(gCanvas);
    gCanvas = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

void setPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    Uint32 *const target = (Uint32 *) ((Uint8 *) surface->pixels
                                                    + y * surface->pitch
                                                    + x * surface->format->BytesPerPixel);
    *target = SDL_MapRGB(surface->format, r, g, b);
}

void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    float slope = (float)(y2 - y1) / (float)(x2 - x1);

    int lastY = y1;
    for (int i = x1; i < x2; i++) {
        for (int j = lastY; j < y1 + i*slope; j++) {
            setPixel(
                surface,
                i, j,
                r, g, b
            );
        }
        lastY = y1 + i*slope;
    }
}

int main(int argc, char *argv[]) {
    if (!init()) {
        printf("Failed to initialize\n");
        return;
    }

    if (!createSurface()) {
        printf("Failed to load media\n");
        return;
    }

    int abort = 0;
    SDL_Event e;

    int x2 = 50;
    int y2 = 50;

    //game loop
    while (!abort) {
        //event loop
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) abort = 1;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        abort = 1;
                        break;
                    case SDLK_w:
                        y2 -= 2;
                        break;
                    case SDLK_a:
                        x2 -= 2;
                        break;
                    case SDLK_s:
                        y2 += 2;
                        break;
                    case SDLK_d:
                        x2 += 2;
                        break;
                }
            }
        }

        //begin render
        SDL_FillRect(gScreenSurface, NULL, 0x000000);
        SDL_FillRect(gCanvas, NULL, 0x000000);

        //render
        drawLine(gCanvas, 0, 0, x2, y2, 0, 255, 0);

        //end render
        SDL_BlitSurface(gCanvas, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(gWindow);
    }

    quit();
}
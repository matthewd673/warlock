# sdlray

Raycasting *(not tracing)* in C with SDL2.

## Build & Run

sdlray requires [SDL2](https://libsdl.org/download-2.0.php), I'm using the mingw version for Windows.

Before building, the working directory should appear as follows:
```
include/
    SDL2/
        (contents of SDL2 include dir)
lib/
    (contents of SDL2 lib dir)
main.c
SDL2.dll
```

**Build command:** `gcc main.c -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2 -o program`

**To run:** `./program`
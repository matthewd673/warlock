# sdlray

Raycasting *(not tracing)* in C with SDL2.

While the project uses SDL2 for rendering, it is intentionally designed to use as few of
SDL's features as possible. I'm not sure why, it just seemed more fun that way.

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

**To debug:**

[Dr. Memory](https://github.com/DynamoRIO/drmemory) is a good alternative to Valgrind for Windows

`drmemory -brief -batch -- program`

## World Files

World files are very rudimentary, they consist of one-letter commands followd by arguments.

- **Load textures:**
  - **Allocate the correct # of texture slots:** `A [ct]`
  - **Load a texture:** `L [filepath]`
- **Set the current texture:** `T 1` (1-indexed, same order they were loaded in)
- **Define a new edge:** `E [x1] [y1] [x2] [y2]`
- Any line starting with `#` is ignored

Each edge (wall) will be textured according to the last texture command *(default: 1)*.
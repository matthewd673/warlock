# warlock

Raycasting *(not tracing)* in C with raylib.

## Build & Run

Warlock requires [raylib](https://www.raylib.com/index.html) and nothing else.

I'm using macOS but Warlock runs anywhere that raylib does (and raylib runs virtually everywhere).

**Build & run commands**

```
cc *.c `pkg-config --libs --cflags raylib` -o game
./game
```

## World Files

World files are very rudimentary, they consist of one-letter commands followd by arguments.

- **Load textures:**
  - **Allocate the correct # of texture slots:** `A [ct]`
  - **Load a texture:** `L [filepath]`
- **Set the current texture:** `T 1` (1-indexed, same order they were loaded in)
- **Define a new edge:** `E [x1] [y1] [x2] [y2]`
- Any line starting with `#` is ignored

Each edge (wall) will be textured according to the last texture command *(default: 1)*.
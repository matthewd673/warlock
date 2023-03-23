# warlock

Warlock is a raycasting *(not tracing)* engine built in C with raylib. Currently, it is most comparable to the Wolf3D renderer except with textured floors and ceilings.

## Build & Run

Warlock requires [raylib](https://www.raylib.com/index.html) and nothing else.

I'm using macOS but Warlock should run anywhere that raylib does (and raylib runs virtually everywhere).

**Build & run commands**

```
cc *.c `pkg-config --libs --cflags raylib` -o game
./game
```

## World Files

World files are very rudimentary, they consist of one-letter commands 
followed by arguments.

- **Load textures:**
  - **Allocate the correct number of texture slots:** `A [ct]`
  - **Load a texture:** `L [filepath]`
- **Set the current wall texture:** `T [texi]` (where `texi` is a texture index, 0-indexed and same order they were loaded in)
- **Define a new edge:** `E [x1] [y1] [x2] [y2]`
- **Set the floor texture:** `F [texi]`
- **Set the ceiling texture:** `C [texi]`
- Any line starting with `#` is ignored

Each edge (wall) will be textured according to the last texture command *(default: 0)*.

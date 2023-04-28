# warlock

Warlock is a raycasting engine built in C with raylib. Currently, it is most comparable to the Wolf3D renderer except with textured floors and ceilings.

**NOTE:** Contrary to popular belief, DOOM used an object-first renderer *not* a raycasting engine.
This means that instead of casting rays to calculate wall heights like Wolf3D it iterated through a tree of walls and projected them to the screen
using some clever trig functions. In other words, Warlock is currently failing in its goal to be a DOOM-like renderer.
The `object-first` branch aims to (eventually) fix this.

## Build & Run

Warlock requires [raylib](https://www.raylib.com/index.html) and nothing else.

I'm using macOS but Warlock should run anywhere that raylib does (and raylib runs virtually everywhere).

**Build & run commands**

```
cd warlock
cc *.c `pkg-config --libs --cflags raylib` -o game
./game
```

## golem

Golem is a tool to build warlock levels. Like Warlock, Golem only needs raylib to build.

**Build & run commands**
```
cd golem
cc *.c `pkg-config --libs --cflags raylib` -o editor
./editor
```

## World Files

World files are very rudimentary, they consist of one-letter commands
followed by arguments.

**NOTE:** World files are gross. Someday, when golem is finished, they will be less gross.

- **Load textures:**
  - **Allocate the correct number of texture slots:** `A [ct]`
  - **Load a texture:** `L [filepath]`
- **Set the current wall texture:** `T [texi]` (where `texi` is a texture index, 0-indexed and same order they were loaded in)
- **Define a new edge:** `E [x1] [y1] [x2] [y2]`
- **Set the floor texture:** `F [texi]`
- **Set the ceiling texture:** `C [texi]`
- Any line starting with `#` is ignored

Each edge (wall) will be textured according to the last texture command *(default: 0)*.

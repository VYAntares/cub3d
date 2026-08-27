# cub3D

A textured raycasting engine in C, rendering a pseudo-3D first-person view from a 2D grid
map — the Wolfenstein 3D technique, rebuilt from scratch on MiniLibX.

---

## About

`cub3D` is the graphics capstone of the [42 School](https://42.fr) core curriculum, and a
team project: two students, one engine. It takes the tile-based world of
[so_long](https://github.com/VYAntares/so_long) and stands it up into three dimensions —
using no 3D library, no OpenGL calls, and no depth buffer.

The renderer is a **raycaster**. For each of the 1280 columns of the window it casts a
single ray from the player's position, walks it through the grid until it hits a wall,
measures the distance, and draws one vertical textured strip whose height is inversely
proportional to that distance. Sixty times a second. Every pixel is written by hand into a
raw framebuffer.

Around the renderer sits a **scene parser** that has to accept a small configuration
language and reject every malformed variation of it, and an **input system** built on key
press/release state so that movement is smooth rather than stuttering one step per
keystroke.

Written in C99 under `-Wall -Wextra -Werror`, conforming to the **Norm**, 42's coding
standard.

---

## Getting MiniLibX

**MiniLibX is not bundled in this repository** — it is 42's library, not mine, so it is not
mine to redistribute. Clone it into the project root before building:

```bash
git clone https://github.com/42Paris/minilibx-linux.git
cd minilibx-linux && ./configure && cd ..
```

On Debian/Ubuntu you will also need the X11 development headers:

```bash
sudo apt install gcc make xorg libxext-dev libbsd-dev
```

The Makefile expects the directory to be named `minilibx-linux`.

---

## Build

```bash
make          # builds ./cub3D
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # fclean + make
```

---

## Usage

```bash
./cub3D maps/good/map_1.cub
```

### Controls

| Key | Action |
|---|---|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `←` | Rotate camera left |
| `→` | Rotate camera right |
| `ESC` | Quit |
| Window close button | Quit |

The window is 1280 × 720. Movement and rotation are driven by held-key state rather than
by discrete keypress events, so holding a key moves continuously and diagonal movement
works by holding two.

---

## Scene format

A `.cub` file has two parts: identifiers, then the map. Identifiers may appear in any
order, but every one of them is mandatory, and the map must come last.

```
NO ./assets/north.xpm
SO ./assets/south.xpm
WE ./assets/west.xpm
EA ./assets/east.xpm

F 60,60,60
C 120,180,255

111111
100001
10N001
100001
111111
```

| Identifier | Meaning |
|---|---|
| `NO` `SO` `WE` `EA` | Path to the XPM texture for each wall facing |
| `F` | Floor colour, as `R,G,B` with each component in `0–255` |
| `C` | Ceiling colour, same format |

| Map character | Meaning |
|---|---|
| `1` | Wall |
| `0` | Open floor |
| `N` `S` `E` `W` | Player start position, and the direction they face |
| space | Void — outside the playable area |

The map does **not** have to be rectangular, which is what makes validating it harder than
in `so_long`: a short row is padded with void, and the enclosure check has to prove that no
open tile is ever adjacent to void or to the edge of the file.

### Errors handled

Every one of these produces a readable message on stderr and a clean exit, with no leaks:

- File extension other than `.cub`, or a file that cannot be opened
- A missing, duplicated, or unknown identifier
- A texture path that does not resolve or is not a valid XPM
- Malformed RGB — wrong component count, non-numeric, or out of the `0–255` range
- Invalid characters in the map
- Zero players, or more than one
- A map that is not fully enclosed by walls
- An empty line inside the map body

`maps/bad/` holds maps built to trip these paths.

---

## How the renderer works

**1. Cast one ray per column.** For screen column `x`, the ray direction is the camera
direction plus the camera plane scaled by `2x / width - 1`. The plane's length sets the
field of view.

**2. Walk the grid with DDA.** Digital Differential Analysis steps the ray from one grid
line to the next — never sub-sampling, never overshooting. `delta_dist` is the ray length
per unit step on each axis; each iteration advances along whichever axis has the nearer
next boundary, so the loop touches exactly the cells the ray actually crosses and stops on
the first `1`.

```c
if (ray->ray_dir_x == 0)
    ray->delta_dist_x = 1e30;        /* a vertical ray never crosses an x boundary */
else
    ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
```

**3. Correct the distance.** Using the raw euclidean distance would bow the walls outward
at the edges of the screen — the fisheye effect. Projecting the hit onto the camera
direction instead gives the *perpendicular* distance, which keeps flat walls flat.

**4. Draw the column.** Wall height on screen is `screen_height / perp_dist`. The side of
the cell that was hit selects the texture (`NO`/`SO`/`WE`/`EA`); the exact hit coordinate
within the cell gives the texture's `x`; stepping down the strip samples texture `y`.
Above the wall is ceiling colour, below it is floor colour.

**5. Blit once.** The whole frame is composed in an off-screen image and pushed to the
window in a single `mlx_put_image_to_window` — one blit per frame, no tearing.

---

## Project structure

```
cub3d/
├── srcs/
│   ├── main.c
│   ├── parsing/            # .cub identifiers, colours, map, and validation
│   ├── init/               # MLX setup, texture loading, player orientation
│   ├── engine/             # loop, key press/release hooks, movement, rotation
│   ├── render/             # raycast, DDA, textured column drawing, background
│   └── utils/              # errors, cleanup, string helpers, get_next_line
├── includes/
│   └── cub3d.h
├── assets/                 # north / south / east / west wall textures (.xpm)
├── maps/
│   ├── good/
│   └── bad/
└── Makefile
```

---

## Concepts covered

- Raycasting and the DDA grid-traversal algorithm
- Perspective projection, and the fisheye correction that makes it look right
- Texture mapping: choosing a face from the hit side, sampling by hit coordinate
- Direct framebuffer writes and double buffering
- Vector rotation for camera control
- Held-key input state versus discrete keypress events
- Parsing and validating a small configuration format from untrusted input
- Strict memory discipline across every exit path

MiniLibX itself reports uninitialised reads under Valgrind. Those come from the library's
own X11 handling, not from this code.

---

## Possible improvements

Minimap · doors · sprites and enemies · mouse look · floor and ceiling texturing ·
multithreaded column rendering

---

## A note on the commit history

This project was originally developed and submitted on **Vogsphere**, the self-hosted
Git server 42 uses internally for project submission and peer evaluation. Vogsphere
repositories are provisioned per student, per project, and live on the school's
infrastructure rather than on a public forge — so what you see here is the final graded
snapshot imported into GitHub, not the day-to-day commit history. The code is exactly
what was submitted and defended during evaluation.

---

## License

Educational project, released as-is for reference. MiniLibX is the property of 42 and is
not included here.

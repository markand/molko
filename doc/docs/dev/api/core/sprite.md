# Module: sprite

Synopsis

```c
#include <core/sprite.h>
```

The sprite is a module to help rendering a large image that is split into
individual parts. This improves memory usage as several images are loaded in a
unique one instead of individual parts.

!!! important
    The image may not contain space, margins or padding within each cell.

## Structs

### sprite

| Field               | Access | Type                     |
|---------------------|--------|--------------------------|
| [texture](#texture) | (+&)   | `struct texture *`       |
| [cellw](#cellw)     | (-)    | `unsigned int `          |
| [cellh](#cellh)     | (-)    | `unsigned int `          |
| [nrows](#nrows)     | (-)    | `unsigned int `          |
| [ncols](#ncols)     | (-)    | `unsigned int `          |

#### texture

The texture to access.

#### cellw

Width of individual cells.

#### cellh

Height of individual cells.

#### nrows

Number of rows computed.

#### ncols

Number of columns computed.

## Functions

### sprite\_init

Initialize a sprite pointed by `sprite` with the texture `tex`.

Arguments `cellw` and `cellh` must specify the dimensions of individual cells in
the image.

The fields `nrows` and `ncols` will be determined automatically from the
texture size.

```c
void
sprite_init(struct sprite *sprite, struct texture *tex, unsigned int cellw, unsigned int cellh)
```

### sprite\_ok

Returns true if the `sprite` is properly initialized.

```c
bool
sprite_ok(const struct sprite *sprite)
```

### sprite\_draw

Draw an individual cell from row `r` and column `c` at the coordinates `x`, `y`
from the sprite `sprite`.

!!! caution
    Argument `r` and `c` must be out of bounds.

```c
bool
sprite_draw(const struct sprite *sprite, unsigned int r, unsigned int c, int x, int y)
```

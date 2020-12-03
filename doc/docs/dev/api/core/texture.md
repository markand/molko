# Module: texture

Synopsis

```c
#include <core/texture.h>
```

Low level texture management.

## Enums

### texture\_blend

Determine the color and alpha transparency to modulate while rendering the
texture.

| Enumerator               | Descriptor           |
|--------------------------|----------------------|
| `TEXTURE_BLEND_NONE`     | No pixel modulation. |
| `TEXTURE_BLEND_BLEND`    | Blend transparency.  |
| `TEXTURE_BLEND_ADD`      | Additive blending.   |
| `TEXTURE_BLEND_MODULATE` | Color modulation.    |

## Structs

### texture

Platform dependant rendering texture.

| Field   | Access | Description    |
|---------|--------|----------------|
| [w](#w) | (-)    | `unsigned int` |
| [h](#h) | (-)    | `unsigned int` |

#### w, h

Texture dimensions.

## Functions

### texture\_new

Create a new texture in `tex` with `w` and `h` as dimensions. Returns false on
errors, in this case `tex` remains uninitialized and must not be used.

```c
bool
texture_new(struct texture *tex, unsigned int w, unsigned int h)
```

### texture\_ok

Returns true if the texture `tex` is properly initialized.

```c
bool
texture_ok(const struct texture *tex)
```

### texture\_set\_blend\_mode

Set the blend mode to `blend` for future blend operations for the texture `tex`.

```c
bool
texture_set_blend_mode(struct texture *tex, enum texture_blend blend)
```

### texture\_set\_alpha\_mod

Apply the transparency `alpha` modulation to the texture `tex`. Returns false on
errors.

!!! note
    You may need to use [texture_set_blend_mode](#texture_set_blend_mode) before
    this function to work.

```c
bool
texture_set_alpha_mod(struct texture *tex, unsigned int alpha)
```

### texture\_set\_color\_mod

Apply the color `color` modulation to the texture `tex`. Returns false on
errors.

```c
bool
texture_set_color_mod(struct texture *tex, unsigned long color)
```

### texture\_draw

Draw whole texture `tex` at the position `x`, `y`.

```c
bool
texture_draw(const struct texture *tex, int x, int y)
```

### texture\_scale

More advanced texture drawing.

Copy the texture `tex` source rectangle specified by `src_x`, `src_y`, `src_w`,
`src_h` at the region `dst_x`, `dst_y`, `dst_w`, `dst_h` with an optional
`angle`.

Returns false on rendering error.

```c
bool
texture_scale(const struct texture *tex,
              int src_x,
              int src_y,
              unsigned src_w,
              unsigned src_h,
              int dst_x,
              int dst_y,
              unsigned dst_w,
              unsigned dst_h,
              double angle)
```

### texture\_finish

Close the texture `tex`.

```c
void
texture_finish(struct texture *tex)
```

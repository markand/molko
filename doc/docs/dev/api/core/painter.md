# Module: painter

Synopsis

```c
#include <core/painter.h>
```

Low level basic graphics primitives.

This module is used to draw on the window or on a specific
[texture](texture.md).

!!! important
    You must open a [window](window.md#window_open) prior to any rendering
    operation.

## Macros

### PAINTER\_BEGIN

Start a block to change the rendering context on the texture `tex`.

!!! note
    Since this macro starts a new scoped block, you may redefine new variables.

```c
#define PAINTER_BEGIN(tex)
```

### PAINTER\_END

Terminate the rendering on the current texture.

```c
#define PAINTER_END()
```

## Functions

### painter\_get\_target

Get the current `texture` used for the rendering context, if NULL is returned
the rendering operates directly on the window.

```c
struct texture *
painter_get_target(void)
```

### painter\_set\_target

Use `tex` as the rendering context for next drawing operations.

If `tex` is NULL, rendering will operate on the global window instead.

!!! important
    You should keep a local texture of the previous target and restore it once
    you're done because the previous stack frame may have set a rendering target
    too.

```c
void
painter_set_target(struct texture *tex)
```

### painter\_get\_color

Return the current drawing color.

```c
unsigned long
painter_get_color(void)
```

### painter\_set\_color

Set the drawing `color` for the next operations.

```c
void
painter_set_color(unsigned long color)
```

### painter\_draw\_line

Draw a line that starts at `x1`, `y1` and ends at `x2`, `y2`.

```c
void
painter_draw_line(int x1, int y1, int x2, int y2)
```

### painter\_draw\_point

Draw a point at `x`, `y`.

```
void
painter_draw_point(int x, int y)
```

### painter\_draw\_rectangle

Draw and fill a rectangle at `x`, `y` of dimensions `w`, `h`.

```c
void
painter_draw_rectangle(int x, int y, unsigned int w, unsigned int h)
```

### painter\_draw\_circle

Draw and fill a circle at the coordinates `x`, `y` of radius `r`.

```c
void
painter_draw_circle(int x, int y, int r)
```

### painter\_clear

Clear the current target.

```c
void
painter_clear(void)
```

### painter\_present

Present the rendering.

!!! note
    Make sure to call this function only once in your game loop.

```c
void
painter_present(void)
```

## Examples

### Block contexts

When rendering on textures, it's always safer to use
[PAINTER_BEGIN](#painter_begin) and [PAINTER_END](#painter_end) to avoid messing
with the global context.

```c
PAINTER_BEGIN(&my_world_texture);
painter_set_color(0xffffffff);
painter_draw_rectangle(0, 0, my_world_texture.w, my_world_texture.h);
PAINTER_END()
```

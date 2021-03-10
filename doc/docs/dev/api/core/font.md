# Module: font

Synopsis

```c
#include <core/font.h>
```

Open and use fonts for rendering UTF-8 text.

## Enums

### font\_style

Specify the font style to use.

| Enumerator               | Description                  |
|--------------------------|------------------------------|
| `FONT_STYLE_ANTIALIASED` | Pretty antialiasing looking. |
| `FONT_STYLE_NONE`        | No antialiasing.             |

## Structs

### font

A font handle to render text.

| Field           | Access | Type              |
|-----------------|--------|-------------------|
| [style](#style) | (+)    | `enum font_style` |

#### style

Font [style](#font_style) to use for the next rendering operations.

## Functions

### font\_open

Open and load `font` of the given pixel `size` from file `path`.

Returns -1 in case of errors and 0 otherwise.

```c
int
font_open(struct font *font, const char *path, unsigned int size)
```

### font\_openmem

Open font and load `font` from the const memory buffer pointed by `buffer` and
of size `buflen` using a pixel size of `size`.

Returns -1 in case of errors and 0 otherwise.

!!! note
    The argument `buffer` must stay valid until the font is no longer used.

```c
int
font_openmem(struct font *font, const void *buffer, size_t buflen, unsigned int size)
```

### font\_ok

Returns non-zero if the `font` is properly loaded.

```c
int
font_ok(const struct font *font)
```

### font\_render

Render the UTF-8 `text` into the texture `tex` using the font pointed by `font`.
The foreground `color` will be used to draw the text.

Returns -1 in case of rendering error, in this case `tex` remains
uninitialized and must not be used.

```c
int
font_render(struct font *font, struct texture *tex, const char *text, unsigned int color)
```

### font\_height

Returns the maximum glyph height in pixels present in `font`.

```c
unsigned int
font_height(const struct font *font)
```

### font\_query

Query the dimensions that the UTF-8 `text` would require with this `font`. Store
the dimensions into the `w`, `h` pointers which can be both NULL.

Returns -1 in case of errors, in this case `w` and `h` are set to 0 if they are
not null.

```c
int
font_query(const struct font *font, const char *text, unsigned int *w, unsigned int *h)
```

### font\_finish

Close this `font`.

```c
void
font_finish(struct font *font)
```

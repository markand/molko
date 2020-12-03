# Module: color

Synopsis

```c
#include <core/color.h>
```

Color conversions.

Within the whole API, colors are managed through a 32 bits unsigned integer and
can be converted back-and-forth using macros from this module.

## Macros

### COLOR\_R

Returns red component of hexadecimal color `c`.

```c
#define COLOR_R(c)
```

### COLOR\_G

Returns green component of hexadecimal color `c`.

```c
#define COLOR_G(c)
```

### COLOR\_B

Returns blue component of hexadecimal color `c`.

```c
#define COLOR_B(c)
```

### COLOR\_A

Returns alpha component of hexadecimal color `c`.

```c
#define COLOR_A(c)
```

### COLOR\_HEX

Convert `r`, `g`, `b`, `a`  components into a hexadecimal color.

```c
#define COLOR_HEX(r, g, b, a)
```

## Examples

### Convert from hexadecimal to RGBA

```c
unsigned long color = 0x12feacff;
unsigned char r, g, b, a;

r = COLOR_R(color);
g = COLOR_G(color);
b = COLOR_B(color);
a = COLOR_A(color);
```

### Convert from RGBA to hexadecimal

```c
unsigned int r = 40;
unsigned int g = 60;
unsigned int b = 100;
unsigned int a = 255;
unsigned long color = COLOR_HEX(r, g, b, a);
```

# Module: maths

Synopsis

```c
#include <core/maths.h>
```

Basic maths.

## Functions

### maths\_is\_boxed

Check if the coordinates `px`, `py` are withing the bounding rectangle specified
by `x`, `y`, `w` and `h`.

```c
bool
maths_is_boxed(int x, int y, unsigned int w, unsigned int h, int px, int py)
```

### maths\_scale

Scale a the value `v` from the range `[omin..omax]` to `[nmin..nmax]`.

```c
float
maths_scale(float v, float omin, float omax, float nmin, float nmax)
```

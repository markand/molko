# Module: image

Synopsis

```c
#include <core/image.h>
```

Load images from disk or memory and convert them as [textures](texture.md).

## Functions

### image\_open

Load the image from `path` on disk and convert it as a texture into `tex`.

Returns -1 in case of errors and 0 otherwise.

```c
int
image_open(struct texture *tex, const char *path)
```

### image\_openmem

Load the image from memory pointed by `buffer` of size `size` and convert it as
a texture into `tex`.

!!! note
    The argument `buffer` must stay valid until the font is no longer used.

```c
int
image_openmem(struct texture *tex, const void *buffer, size_t size)
```

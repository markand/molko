# Module: drawable

Synopsis

```c
#include <core/drawable.h>
```

Automatic drawable objects.

This module allows creating automatic objects that draw theirselves into the
screen and vanish once complete. They could be considered as lightweight
alternatives to [actions](action.md), however in contrast to them, drawables do
have a position.

## Macros

### DRAWABLE\_STACK\_MAX

Maximum number of drawable in a unique [drawable_stack](#drawable_stack).

```c
#define DRAWABLE_STACK_MAX      128
```

## Structs

### drawable

Abstract drawable object.

| Field             | Access | Type                                        |
|-------------------|--------|---------------------------------------------|
| [data](#data)     | (+&?)  | `void *`                                    |
| [x](#x)           | (+)    | `int`                                       |
| [y](#y)           | (+)    | `int`                                       |
| [update](#update) | (+?)   | `int (*)(struct drawable *, unsigned int)` |
| [draw](#draw)     | (+?)   | `void (*)(struct drawable *)`               |
| [end](#end)       | (+?)   | `void (*)(struct drawable *)`               |
| [finish](#finish) | (+?)   | `void (*)(struct drawable *)`               |

#### data

Optional drawable data.

#### x, y

Position on screen.

#### update

Update the drawable `self` with the `ticks` since last frame. The callback
should return non-zero if it is considered complete.

```c
int (*update)(struct drawable *self, unsigned int ticks)
```

#### draw

Draw the drawable `self`.

```c
void (*draw)(struct drawable *self)
```

#### end

Called with the drawable `self` when it was completed.

This callback is mostly provided to allow the user doing something else once an
drawable is complete. Predefined drawable should not use this callback by
themselves.

```c
void (*end)(struct drawable *self)
```

#### finish

Destroy internal resources for the drawable `self`.

```c
void (*finish)(struct drawable *self)
```

### drawable\_stack

Stack of drawable objects.

This stack of drawable object can be used to store drawable objects within a
specific transition (state, battle, menu, etc).

You can add, clear, update and draw them.

| Field               | Access | Type                                    |
|---------------------|--------|-----------------------------------------|
| [objects](#objects) | (+&?)  | `struct drawable *[DRAWABLE_STACK_MAX]` |

#### objects

Non-owning array of drawables to manage.

## Functions

### drawable\_update

Invoke and return the `dw` [update callback](#update) with the given event `ev`
and `ticks` since last frame if it is not NULL.

```c
int
drawable_update(struct drawable *dw, unsigned int ticks)
```

### drawable\_draw

Invoke the `dw` [draw callback](#draw) if it is not NULL.

```c
void
drawable_draw(struct drawable *dw)
```

### drawable\_end

Invoke the `dw` [end callback](#end) if it is not NULL.

```c
void
drawable_end(struct drawable *dw)
```

### drawable\_finish

Invoke the `dw` [finish callback](#finish) if it is not NULL.

```c
void
drawable_finish(struct drawable *dw)
```

### drawable\_stack\_init

Initalize the drawable stack `st`.

!!! note
    It is unnecessary if the object was zero'ed.

```
void
drawable_stack_init(struct drawable_stack *st)
```

### drawable\_stack\_add

Add the drawable `dw` to the stack pointed by `st`. Returns -1 if there wasn't
enough room to insert.

```c
int
drawable_stack_add(struct drawable_stack *st, struct drawable *dw)
```

### drawable\_stack\_update

Update all drawables with `ticks` since last frame in the stack `st`.

```c
int
drawable_stack_update(struct drawable_stack *st, unsigned int ticks)
```

### drawable\_stack\_draw

Draw all drawables in the stack `st`.

```c
void
drawable_stack_draw(const struct drawable_stack *st)
```

### drawable\_stack\_completed

Tells if there is any pending drawable in the stack `st`. Returns non-zero if
there are no drawables or if they have all completed.

```c
int
drawable_stack_completed(const struct drawable_stack *st)
```

### drawable\_stack\_finish

Terminate all drawables and clear the stack `st`.

```c
void
drawable_stack_finish(struct drawable_stack *st)
```

# Module: window

Synopsis

```c
#include <core/window.h>
```

Window management.

## Globals

| Variable | Type            |
|----------|-----------------|
| `window` | `struct window` |

## Enums

### window\_cursor

Window mouse cursor.

| Enumerator                | Description               |
|---------------------------|---------------------------|
| `WINDOW_CURSOR_ARROW`     | Standard arrow.           |
| `WINDOW_CURSOR_EDIT`      | Text edit cursor "I".     |
| `WINDOW_CURSOR_WAIT`      | Busy cursor.              |
| `WINDOW_CURSOR_CROSSHAIR` | Cross-hair for selection. |
| `WINDOW_CURSOR_SIZE`      | Size/moving.              |
| `WINDOW_CURSOR_NO`        | Action forbidden.         |
| `WINDOW_CURSOR_HAND`      | Hand.                     |

## Structs

### window

Window structure.

| Field                   | Access | Type           |
|-------------------------|--------|----------------|
| [w](#w-h)               | (-)    | `unsigned int` |
| [h](#w-h)               | (-)    | `unsigned int` |
| [framerate](#framerate) | (-)    | `unsigned int` |

#### w, h

Window dimensions.

#### framerate

Display framerate if supported, otherwise set to 0.

## Functions

## window\_open

Open a window with dimensions `width`, `height`. The argument `title` will set
the window title if the platform supports it.

Returns false on errors, otherwise the global `window` object will be set.

```c
bool
window_open(const char *title, unsigned int width, unsigned int height)
```

## window\_set\_cursor

Change the window cursor to `cursor`.

```c
void
window_set_cursor(enum window_cursor cursor)
```

## window\_finish

Close the window.

!!! caution
    Many modules expect a window to be running, do not attemps to use the API
    after calling this function.

```c
void
window_finish(void)
```

# Module: state

Synopsis

```c
#include <core/state.h>
```

The state module is a facility that allows changing game context with ease using
a single [game_switch](game.md#game_switch) function.

## Structs

### state

Abstract structure that consists of user callbacks to control the main game
loop.

| Field             | Access | Type                                            |
|-------------------|--------|-------------------------------------------------|
| [data](#data)     | (+&?)  | `void *`                                        |
| [start](#start)   | (+?)   | `void (*)(struct state *)`                      |
| [handle](#handle) | (+?)   | `void (*)(struct state *, const union event *)` |
| [update](#update) | (+?)   | `void (*)(struct state *, unsigned int)`        |
| [draw](#draw)     | (+?)   | `void (*)(struct state *)`                      |
| [end](#end)       | (+?)   | `void (*)(struct state *)`                      |
| [finish](#finish) | (+?)   | `void (*)(struct state *)`                      |

#### data

Optional user data.

```c
void *data
```

#### start

This function is called when the state `self` is about to begin.

```c
void (*start)(struct state *self)
```

#### handle

Call this function for the state `self` for each event `ev` that happened.

```c
void (*handle)(struct state *self, const union event *ev)
```

#### update

Update the state `self` with `ticks` since last frame.

```c
void (*update)(struct state *self, unsigned int ticks)
```

#### draw

Draw the state `self`.

```c
void (*draw)(struct state *self)
```

#### end

This function is called with state `self` when the state is about to be switched
off.

```c
void (*end)(struct state *self)
```

#### finish

Close resources in state `self`.

```c
void (*finish)(struct state *self)
```

## Functions

### state\_start

Invoke the `state` [start callback](#start) if not NULL.

```c
void
state_start(struct state *state)
```

### state\_handle

Invoke the `state` [handle callback](#handle) with the given event `ev` if it is
not NULL.

```c
void
state_handle(struct state *state, const union event *ev)
```

### state\_update

Invoke and return the `state` [update callback](#update) with `ticks` since last
frame if it is not NULL.

```c
void
state_update(struct state *state, unsigned int ticks)
```

### state\_draw

Invoke the `state` [draw callback](#draw) if not NULL.

```c
void
state_draw(struct state *state)
```

### state\_end

Invoke the `state` [end callback](#end) if not NULL.

```c
void
state_end(struct state *state)
```

### state\_finish

Invoke the `state` [finish callback](#finish) if not NULL.

```c
void
state_finish(struct state *state)
```

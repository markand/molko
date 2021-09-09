# Module: game

Synopsis

```c
#include <core/game.h>
```

Main game loop and states.

This module offers a global game structure that contain a [state](state.md). It
is designed to help switching game states and inhibit some of the functions when
necessary.

States are pushed and removed from the stack as a LIFO queue, the last pushed
state will be the first state to be removed.

The main loop use a constant frame rate mechanism based on the screen refresh
rate if supported, otherwise it use a default frame rate of 60hz. In any case,
the frame rate is capped in the main loop and the elapsed time is measured
accordingly to update the game at constant speed no matter the refresh rate is
present.

Since only one game must be present at a time, a global `game` variable is
available for convenience to not carrying it everywhere.

## Globals

| Variable | Type          |
|----------|---------------|
| `game`   | `struct game` |

## Structs

### game

Game structure.

| Field                     | Access | Type             |
|---------------------------|--------|------------------|
| [inhibit](#inhibit)       | (+)    | `enum inhibit`   |

#### inhibit

Current inhibit flags set, see [inhibit](inhibit.md) for more information.

## Functions

### game\_init

Initialize the game. This isn't required unless [game_quit](#game_quit) was
called.

```c
void
game_init(void)
```

### game\_push

Push `state` into the stack. If there is a current running state, it is
suspended through the defined callback.

The argument `state` must remain valid until the lifetime of the game.

```c
void
game_push(struct state *state)
```

### game\_pop

Remove the last state and invoke finalizer callbacks (end, finish). If there is
a previous state into the stack, it is resumed through the defined callback.

```c
void
game_pop(void)
```

### game\_handle

Handle the event `ev` into the current state.

```c
void
game_handle(const union event *ev)
```

### game\_update

Update the current state with `ticks` (in milliseconds) since last frame.

```c
void
game_update(unsigned int ticks)
```

### game\_draw

Draw the current state.

```c
void
game_draw(void)
```

### game\_loop

Start a blocking loop that call in order [game_handle](#game_handle),
[game_update](#game_update) and [game_draw](#game_draw) while keeping a constant
framerate.

```c
void
game_loop(void)
```

### game\_quit

Destroy all states.

Even if you don't use [game_loop](#game_loop) you should call this function
because it will close state resources.

!!! caution
    If you call this function from a state itself, you must return immediately
    because the state will be finalized immediately.

```c
void
game_quit(void)
```

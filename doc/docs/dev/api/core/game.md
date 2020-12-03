# Module: game

Synopsis

```c
#include <core/game.h>
```

Main game loop and states.

This module offers a global game structure that contain a [state](state.md). It
is designed to help switching game states and inhibit some of the functions when
necessary.

In contrast to popular engines, states are not stacked and only one is used at a
time. Switching states mean that the current state will run until next frame and
will be closed afterwards.

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
| [state](#state)           | (+&?)  | `struct state *` |
| [state_next](#state_next) | (+&?)  | `struct state *` |

#### inhibit

Current inhibit flags set, see [inhibit](inhibit.md) for more information.

#### state

Current state running.

#### state\_next

Optional next state to be changed in next frame loop.

## Functions

### game\_switch

Set `state` for the next frame.

The state will only be effective after the next call to
[game_update](#game_update).

If argument `quick` is set to true, the state is changed immediately and the
current state code should immediately return.

```c
void
game_switch(struct state *state, bool quick)
```

#### game\_handle

Handle the event `ev` into the current state.

```c
void
game_handle(const union event *ev)
```

#### game\_update

Update the current state with `ticks` since last frame.

```c
void
game_update(unsigned int ticks)
```

#### game\_draw

Draw the current state.

```c
void
game_draw(void)
```

#### game\_loop

Start a blocking loop that call in order [game_handle](#game_handle),
[game_update](#game_update) and [game_draw](#game_draw) while keeping a constant
framerate.

```c
void
game_loop(void)
```

#### game\_stop

Destroy both next and current state if any.

Even if you don't use [game_loop](#game_loop) you should call this function
because it will close state resources.

!!! caution
    If you call this function from a state itself, you must return immediately
    because the state will be finalized immediately.

```c
void
game_quit(void)
```

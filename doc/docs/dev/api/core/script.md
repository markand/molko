# Module: script

Synopsis

```c
#include <core/script.h>
```

Module to create sequence of actions.

Those routines wrap individual actions into a sequence of actions into an action
itself.

This is convenient for scenarios where you need to specify several sequential
actions that neet to waid the previous before continuing.

In a nutshell, to write a scenario you should:

1. Create a script with see [script_init](#script_init),
2. Create one or more actions and append with [script_append](#script_append).

## Macros

### SCRIPT\_ACTION\_MAX

Maximum number of actions in a script.

```c
#define SCRIPT_ACTION_MAX (128)
```

## Structs

### script

Structure with actions that must be ran in order.

| Field                 | Access | Type                                 |
|-----------------------|--------|--------------------------------------|
| [actions](#action)    | (+&?)  | `struct action *[SCRIPT_ACTION_MAX]` |
| [actionsz](#actionsz) | (+)    | `size_t`                             |
| [cur](#cur)           | (-)    | `size_t`                             |

#### actions

Array of non-owning actions to run in order.

#### actionsz

Number of actions in array [actions](#actions)

#### cur

Current action index.

## Functions

### script\_init

Initialize the script `s`.

This is not necessary if you zero'ed the structure.

```c
void
script_init(struct script *s)
```

### script\_append

Add the action `a` into the script `s`. Returns true if there was enough room
to insert.

!!! note
    The argument `a` must stay valid until the script is no longer used.

```c
bool
script_append(struct script *s, struct action *a)
```

### script\_handle

Handle the event `ev` into the current action in script `s`.

```c
void
script_handle(struct script *s, const union event *ev)
```

### script\_update

Update the current action in the script `s` with `ticks` since last frame.
Returns true if the script completed.

```c
bool
script_update(struct script *s, unsigned int ticks)
```

### script\_draw

Draw the current action in script `s`.

```c
void
script_draw(struct script *s)
```

### script\_completed

Returns true if the script `s` is complete.

```c
bool
script_completed(const struct script *s)
```

### script\_finish

Clear the script `s` and all of its actions.

```c
void
script_finish(struct script *s)
```

### script\_action

Convert the script `s` into the action `dst`.

!!! note
    The argument `s` must stay valid until the script is no longer used.

```c
void
script_action(struct script *s, struct action *dst)
```

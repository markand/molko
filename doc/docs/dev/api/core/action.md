# Module: action

Synopsis

```c
#include <core/action.h>
```

Generic updatable and drawable actions.

This module help creating user interaction within the gameplay by adding
actions. They have the following properties:

- Can handle user input and events,
- Can be updated through the game loop,
- Can be drawn.

Most more high level objects can handle actions to add flexibility (like in
battles, maps, etc).

## Macros

### ACTION\_STACK\_MAX

Maximum number of action in a unique [action_stack](#action_stack).

```c
#define ACTION_STACK_MAX 128
```

## Structs

### action

Use this structure to create an action that may handle input, be updated and
drawn.

All members can be NULL.

| Field             | Access | Type                                             |
|-------------------|--------|--------------------------------------------------|
| [data](#data)     | (+&?)  | `void *`                                         |
| [handle](#handle) | (+?)   | `void (*)(struct action *, const union event *)` |
| [update](#update) | (+?)   | `int (*)(struct action *, unsigned int)`        |
| [draw](#draw)     | (+?)   | `void (*)(struct action *)`                      |
| [end](#end)       | (+?)   | `void (*)(struct action *)`                      |
| [finish](#finish) | (+?)   | `void (*)(struct action *)`                      |

#### data

Arbitrary user data.

```c
void *data
```

#### handle

Handle an event.

```c
void (*handle)(struct action *self, const union event *ev)
```

#### update

Update the action `self` with the `ticks` since last frame. The callback should
return non-zero if it is considered complete.

```c
int (*update)(struct action *self, unsigned int ticks)
```

#### draw

Draw the action `self`.

```c
void (*draw)(struct action *self)
```

#### end

Called with the action `self` when it was completed.

This callback is mostly provided to allow the user doing something else once an
action is complete. Predefined actions should not use this callback by
themselves.

```c
void (*end)(struct action *self)
```

#### finish

Destroy internal resources for the action `self`.

Close the action before removal. This function should be used to deallocate
memory if necessary.

```c
void (*finish)(struct action *act)
```

### action\_stack

Stack of actions.

The purpose of this structure is to help managing several actions at once.
Actions are automatically removed from the stack if the corresponding update
member function returns true after completion.

This structure contains pointers to actions that must be kept until the stack is
destroyed. User is responsible of deallocating them if they were allocated from
the heap.

| Field               | Type                                |
|---------------------|-------------------------------------|
| [actions](#actions) | `struct action *[ACTION_STACK_MAX]` |

#### actions

Non-owning array of actions to manage.

## Functions

### action\_handle

Invoke the `act` [handle callback](#handle) with the given event `ev` if it is
not NULL.

```c
void
action_handle(struct action *act, const union event *ev)
```

### action\_update

Invoke and return the `act` [update callback](#update) with `ticks` since last
frame if it is not NULL.

```c
int
action_update(struct action *act, unsigned int ticks)
```

### action\_draw

Invoke the `act` [draw callback](#draw) if it is not NULL.

```c
void
action_draw(struct action *act)
```

### action\_end

Invoke the `act` [end callback](#end) if it is not NULL.

```c
void
action_end(struct action *act)
```

### action\_finish

Invoke the `act` [finish callback](#finish) if it is not NULL.

```c
void
action_finish(struct action *act)
```

### action\_stack\_init

Initalize the action stack `st`.

!!! note
    It is unnecessary if the object was zero'ed.

```
void
action_stack_init(struct action_stack *st)
```

### action\_stack\_add

Add the action `act` to the stack pointed by `st`. Returns -1 if there wasn't
enough room to insert.

```c
int
action_stack_add(struct action_stack *st, struct action *act)
```

### action\_stack\_handle

Handle the event `ev` for all actions in the stack `st`.

```c
void
action_stack_handle(struct action_stack *st, const union event *ev)
```

### action\_stack\_update

Update all actions with `ticks` since last frame in the stack `st`.

```c
int
action_stack_update(struct action_stack *st, unsigned int ticks)
```

### action\_stack\_draw

Draw all actions in the stack `st`.

```c
void
action_stack_draw(const struct action_stack *st)
```

### action\_stack\_completed

Tells if there is any pending action in the stack `st`. Returns non-zero if
there are no actions or if they have all completed.

```c
int
action_stack_completed(const struct action_stack *st)
```

### action\_stack\_finish

Terminate all actions and clear the stack `st`.

```c
void
action_stack_finish(struct action_stack *st)
```

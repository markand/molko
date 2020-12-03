# Module: clock

Synopsis

```c
#include <core/clock.h>
```

Module to keep track of elapsed time.

## Structs

### clock

| Field           | Access | Type           |
|-----------------|--------|----------------|
| [ticks](#ticks) | (+)    | `unsigned int` |

#### ticks

Time point on clock initialization in milliseconds.

## Functions

### clock\_start

Start capturing time in the clock `clock`.

```c
void
clock_start(struct clock *clock)
```

### clock\_elapsed

Returns the elapsed time since the last call to [clock_start](#clock_start) in
the clock `clock`.

```c
unsigned int
clock_elapsed(const struct clock *clock)
```

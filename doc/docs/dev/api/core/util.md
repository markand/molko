# Module: util

Synopsis

```c
#include <core/util.h>
```

Miscellaneous utilities.

## Macros

### NELEM

Get the number of elements in the static C array `x`.

```c
#define NELEM(x) sizeof ((x)) / sizeof ((x)[0])
```

## functions

### delay

Put the thread to sleep for a given amount `ms` milliseconds.

```c
void
delay(unsigned int ms)
```

### pprintf

Construct a temporary path to a file that can fit in a `PATH_MAX` array.

This function is useful when specifying paths into a function invocation such
as `fopen(pprintf("%s.png", i), "r"))`.

Format string is similar to [printf][]

```c
const char *
pprintf(const char *fmt, ...)
```

### nrand

Returns a random number between `lower` and `upper` (included).

```c
unsigned int
nrand(unsigned int lower, unsigned int upper)
```

[printf]: https://en.cppreference.com/w/c/io/fprintf

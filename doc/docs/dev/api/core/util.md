# Module: util

Synopsis

```c
#include <core/util.h>
```

Miscellaneous utilities.

## Macros

### UTIL_SIZE

Get the number of elements in the static C array `x`.

```c
#define UTIL_SIZE(x) sizeof ((x)) / sizeof ((x)[0])
```

## functions

### util_delay

Put the thread to sleep for a given amount `ms` milliseconds.

```c
void
util_delay(unsigned int ms)
```

### util_pathf

Construct a temporary path to a file that can fit in a `PATH_MAX` array.

This function is useful when specifying paths into a function invocation such
as `fopen(pprintf("%s.png", i), "r"))`.

Format string is similar to [printf][]

```c
const char *
util_pathf(const char *fmt, ...)
```

### util_nrand

Returns a random number between `lower` and `upper` (included).

```c
unsigned int
util_nrand(unsigned int lower, unsigned int upper)
```

[printf]: https://en.cppreference.com/w/c/io/fprintf

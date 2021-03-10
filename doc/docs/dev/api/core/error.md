# Module: error

Synopsis:

```c
#include <core/error.h>
```

Retrieve or set last global error.

!!! note
    The error variable is global and not thread safe nor reentrant.

## Functions

### error

Return the last error string.

```c
const char *
error(void)
```

### errorf

Set the global error using [printf][] format string. Also return -1 for
convenience.

```c
int
errorf(const char *fmt, ...)
```

### errorva

Similar to [errorf](#errorf) but using a `va_list` argument instead.

```c
int
errorva(const char *fmt, va_list ap)
```

[printf]: https://en.cppreference.com/w/c/io/fprintf

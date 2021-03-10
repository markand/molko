# Module: core

Synopsis

```c
#include <core/core.h>
```

Main entry point for the libmlk-core library.

The library must be initialized before attempting to call any other functions in
the API.

## Functions

### core\_init

Initialize the library.

You must specify an `organization` and program `name`. The `organization`
argument can be anything but [reversed domain name notation][rdnn] is often
used. It will also determine the path to the preferred directories on certain
platforms so make sure to not use characters that may be illegal on some
filesystems.

Returns -1 in case of errors and 0 otherwise.

```c
int
core_init(const char *organization, const char *name)
```

### core\_finish

Close the library.

```c
void
core_finish(void)
```

[rdnn]: https://en.wikipedia.org/wiki/Reverse_domain_name_notation

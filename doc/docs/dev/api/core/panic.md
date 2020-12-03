# Module: panic

Synopsis

```c
#include <core/panic.h>
```

Unrecoverable error handling.

This set of functions should be used to detect runtime errors that are
unexpected. They should be used only when the game cannot continue because it is
in a unrecoverable state.

Examples of appropriate use cases:

- Game saved data is corrupt,
- Assets are missing,
- No more memory.

In other contexts, use asserts to indicates programming error and appropriate
solutions to recover the game otherwise.

## Globals

| Variable                        | Type             |
|---------------------------------|------------------|
| [panic_handler](#panic_handler) | `void (*)(void)` |

### panic\_handler

Global panic handler.

The default implementation shows the last error and exit with code 1. The
function must not return so you have to implement a setjmp/longjmp or a
exception to be thrown.

If the user defined function returns, panic routines will finally exit with code
1.

## Functions

### panicf

Terminate the program using the [panic_handler](#panic_handler) routine.

This function will first set the global error with the provided format
string using the [printf][] format and then call the handler.

```c
noreturn void
panicf(const char *fmt, ...)
```

### panicva

Like [panicf](#panicf) but using a `va_list`.

```c
noreturn void
panicva(const char *fmt, va_list ap)
```

### panic

Call the [panic_handler](#panic_handler) using the last registered error.

```c
noreturn void
panic(void)
```

[printf]: https://en.cppreference.com/w/c/io/fprintf

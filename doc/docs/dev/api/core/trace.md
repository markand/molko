# Module: trace

Synopsis

```c
#include <core/trace.h>
```

The purpose of this module is to provide a feedback from the code when there are
non-fatal programming error or unexpected results. In contrast to the
[debug](debug.md) module this one is always activated no manner if the build is
in Debug or Release.

For example, having an animation with a delay of 0 is not a technical issue
but is probably not what the use wants. Thus, a trace warning may be
generated in that way.

## Globals

| Variable                        | Type                     |
|---------------------------------|--------------------------|
| [trace_handler](#trace_handler) | `void (*)(const char *)` |
| [trace_data](#trace_data)       | `void *`                 |

### trace\_handler

The default one use a simple printf on the standard output.

### trace\_data

Placeholder data for the handler if needed.

## Macros

### TRACE\_LINE\_MAX

Maximum length for a trace log.

```c
#define TRACE_LINE_MAX (1024)
```

## Functions

### tracef

Log some information using [printf][] format string.

```c
void
tracef(const char *fmt, ...)
```

### traceva

Like [tracef](#tracef) but using a `va_list`.

```c
void
traceva(const char *fmt, va_list ap)
```

[printf]: https://en.cppreference.com/w/c/io/fprintf

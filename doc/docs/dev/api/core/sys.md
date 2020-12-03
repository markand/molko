# Module: sys

Synopsis

```c
#include <core/sys.h>
```

System utilities.

In the API all paths are normalized using forward slashes `/` no matter which
platforms it's running on.

## Enums

### sys\_dir

Kind of special directories.

| Enumerator       | Description                        |
|------------------|------------------------------------|
| `SYS_DIR_BIN`    | Path to binaries.                  |
| `SYS_DIR_DATA`   | Directory containing data.         |
| `SYS_DIR_LOCALE` | Path to NLS catalogs.              |
| `SYS_DIR_SAVE`   | User directory for save databases. |

## Functions

### sys\_init

Initialize the system.

This function is automatically called from [core_init](core.md#core_init) with
the same arguments and not necessary from the user.

```c
bool
sys_init(const char *organization, const char *name)
```

### sys\_dir

Return a system or user directory preferred for this platform for the given
`kind`.

!!! note
    Returned path is never NULL but points to a static storage area.

```c
const char *
sys_dir(enum sys_dir kind)
```

### sys\_mkdir

Create the directory `path` recursively. Returns false on errors other than
already existing.

```c
bool
sys_mkdir(const char *path)
```

### sys\_finish

This function is automatically called from [core_finish](core.md#core_finish)
with the same arguments and not necessary from the user.

```c
void
sys_finish(void)
```

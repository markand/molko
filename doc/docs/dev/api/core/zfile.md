# Module: zfile

Synopsis

```c
#include <core/zfile.h>
```

Open potentially compressed file through [ZSTD][] using C `FILE` interface.

## Structs

### zfile

This structure owns the context for loading the file and must be kept until the
FILE pointer is no longer necessary.

| Field             | Access | Type                                             |
|-------------------|--------|--------------------------------------------------|
| [fp](#fp)         | (+)    | `FILE *`                                         |

#### fp

The C file pointer to read. It is opened in read-only text mode and must not be
closed, free'd or anything else.

## Functions

### zfile\_open

Open a file from `path` which can be compressed using [ZSTD][] or kept plain. If
compressed the data is decompressed transparently and stored into the structure
context `zf`.

Returns -1 on failure and 0 on success, in this case you can access the `fp`
field member to perform read operations.

```c
int
zfile_open(struct zfile *zf, const char *path)
```

### zfile\_close

Close the file context `zf`.

```c
void
zfile_close(struct zfile *zf)
```

[ZSTD]: http://zstandard.org

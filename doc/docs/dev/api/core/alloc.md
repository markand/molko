# Module: alloc

Synopsis

```c
#include <core/alloc.h>
```

This module controls how the API should allocate memory. Although dynamic
allocation isn't much used in the core API, it is used in few places where
static arrays would not fit (e.g. loading maps).

## Macros

### ALLOC\_POOL\_INIT\_DEFAULT

Default size to allocate in struct alloc_pool.

!!! warning
    Must be a power of 2.

```c
#define ALLOC_POOL_INIT_DEFAULT 32
```

## Structs

### alloc\_funcs

Allocator functions.

!!! warning
    When changing allocator using [alloc_set](#alloc_set) all functions must
    be set.

| Field               | Access | Type                                        |
|---------------------|--------|---------------------------------------------|
| [alloc](#alloc)     | (+?)   | `void *(*)(size_t)`                         |
| [realloc](#realloc) | (+?)   | `void *(*)(void *, size_t)`                 |
| [free](#free)       | (+?)   | `void (*)(void *)`                          |

#### alloc

Allocate the given `size` of bytes. Returns the allocated memory (or NULL on
failure).

The default implementation uses malloc and calls [panic](panic.md) in case of
failure.

!!! note
    You should set an [error](error.md) in case of failure.

```c
void *(*alloc)(size_t size)
```

#### realloc

Realloc the region `ptr` with the new given `size`. Returns the new memory (may
be NULL).

The default implementation uses malloc and calls [panic](panic.md) in case of
failure.

!!! note
    You should set an [error](error.md) in case of failure.

```c
void *(*realloc)(void *ptr, size_t size)
```

#### free

Free memory pointed by `ptr`.

The default implementation calls standard C free function.

```c
void (*free)(void *ptr)
```

### alloc\_pool

Pool allocator.

This small structure is a helper to reallocate data each time a new slot is
requested. It allocates twice as the current storage when size exceeds
capacity.

It uses realloc mechanism to upgrade the new storage space so pointers
returned must not be referenced directly.

It is designed in mind to help allocating resources locally that may be
referenced in another module without having to manage an array from the user
code. Because it is designed for this responsability only it only supports
insertion.

The initial capacity is controlled by the
[ALLOC\_POOL\_INIT\_DEFAULT](#macro-alloc_pool_init_default) macro and **must**
be a power of two.

A custom finalizer function can be set to finalize each individual object if
necessary.

| Field                 | Access | Type     |
|-----------------------|--------|----------|
| [data](#data)         | (+?)   | `void *` |
| [elemsize](#elemsize) | (-)    | `size_t` |
| [size](#size)         | (-)    | `size_t` |
| [capacity](#capacity) | (-)    | `size_t` |

#### data

Pointer to the region.

```c
void *data
```

#### elemsize

Size of individual element.

```c
size_t elemsize
```

#### size

Number of items in array.

```c
size_t size
```

#### capacity

Current capacity.

```c
size_t capacity
```

#### finalizer

Optional finalizer that should finalize the object pointed by `data`.

This function will be invoked for every element when
[alloc\_pool\_finish](#alloc_pool_finish) is called.

```c
void (*finalizer)(void *data)
```

## Functions

### alloc\_set

Use [`funcs`](#alloc_funcs) as the new allocator routines. It must be kept valid
until the program is no longer used.

```c
void
alloc_set(const struct alloc_funcs *funcs)
```

### alloc\_new

Allocate new uninitialized data of the given `size`. Returns the result of the
current allocator [alloc](#alloc) function set.

```c
void *
alloc_new(size_t size)
```

### alloc\_new0

Invoke [alloc_new](#alloc_new) but zero initialize the memory.

```c
void *
alloc_new0(size_t size)
```

### alloc\_array

Allocate an uninitialized array of `len` elements of `elemsize` individually.
Returns the result of the current [alloc](#alloc) function set.

```c
void *
alloc_array(size_t len, size_t elemsize)
```

### alloc\_array0

Invoke [alloc_array](#alloc_array) but zero initialize the memory.

```c
void *
alloc_array0(size_t len, size_t elemsize)
```

### alloc\_renew

Reallocate the pointer `ptr` (which may be NULL) to the new `size` size. The
size can be 0. Returns the result of the current [alloc](#alloc) function set.

```c
void *
alloc_renew(void *ptr, size_t size)
```

### alloc\_rearray

Reallocate the pointer `ptr` (which may be NULL) as an array of `newlen`
elements of `elemsize` individually. Returns the result of the current
[realloc](#realloc) function set.

```c
void *
alloc_rearray(void *ptr, size_t newlen, size_t elemsize)
```

### alloc\_rearray0

Reallocate the `ptr` (which maybe NULL) with the `oldlen` as current number of
elements of `elemsize` to the `newlen`. If the `newlen` is greater than oldlen,
the new region is cleared with 0. Returns the result of the current
[realloc](#realloc) function set.

```c
void *
alloc_rearray0(void *ptr, size_t oldlen, size_t newlen, size_t elemsize)
```

### alloc\_dup

Duplicate the `ptr` region with the given `size`.

```c
void *
alloc_dup(const void *ptr, size_t size)
```

### alloc\_sdup

Duplicate the string `src`. Returns the result of current [alloc](#alloc)
function set.

```c
char *
alloc_sdup(const char *src)
```

### alloc\_sdupf

Create a dynamically allocated string using printf(3) format like.

```c
char *
alloc_sdupf(const char *fmt, ...);
```

### alloc\_pool\_init

Initialize the `pool` as an array where elements have `elemsize` size. Optional
[`finalizer`](#finalizer) argument can be passed to finalize every element when
clearing the pool.

This will effectively create a initial storage according to
[ALLOC_POOL_INIT_DEFAULT](#alloc_pool_init_default).

Returns -1 on error depending on the result of the of the current
[alloc](#alloc) function set or 0 otherwise.

```c
bool
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *))
```

### alloc\_pool\_new

Request a new slot from the `pool`.

If the current size has reached the capacity, it will be doubled in that case it
is possible that all previous pointer may be invalidated.

Returns NULL on errors depending on the result of the of the current
[realloc](#realloc) function set.

```c
void *
alloc_pool_new(struct alloc_pool *pool)
```

### alloc\_pool\_get

Get the value at the given `index` from the `pool`.

!!! warning
    Undefined behavior if `index` is out of bounds.

```c
void *
alloc_pool_get(const struct alloc_pool *pool, size_t index)
```

### alloc\_pool\_finish

inalize the `pool` and all individual element if a finalizer is set.

You must call [alloc\_pool\_init](#alloc_pool_init) again before reusing it.

```c
void
alloc_pool_finish(struct alloc_pool *pool)
```

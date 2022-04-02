# Module: alloc

Synopsis

```c
#include <core/alloc.h>
```

This module controls how the API should allocate memory. Although dynamic
allocation isn't much used in the core API, it is used in few places where
static arrays would not fit (e.g. loading maps).

All of these functions are expected to never return NULL in case of memory
exhaustion but to call [panic][] instead as recovering from out-of-memory in a
game is near impossible.

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

Allocate the given `size` of bytes and return the memory region.

The default implementation uses malloc and calls [panic][] in case of failure.

!!! note
    You should also set an [error](error.md) in case of failure.

```c
void *(*alloc)(size_t size)
```

#### realloc

Realloc the region `ptr` with the new given `size` and return the memory region.

The default implementation uses malloc and calls [panic][] in case of failure.

!!! note
    You should also set an [error](error.md) in case of failure.

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

Array allocator.

This small structure is a helper to reallocate an array each time a new element
is requested. It allocates twice as the current storage when size exceeds
capacity.

It uses realloc mechanism to upgrade the new storage space so pointers
returned in [alloc_pool_new](#alloc_pool_new) may get invalided when this
function is called.

It is designed in mind to help allocating an array of elements when they can't
be determined at runtime (e.g. while reading a file) without the performance
cost of using [alloc_rearray](#alloc_rearray) every elements.

The initial capacity is controlled by the
[ALLOC\_POOL\_INIT\_DEFAULT](#macro-alloc_pool_init_default) macro and **must**
be a power of two.

A custom finalizer function can be set to finalize each individual object if
necessary.

| Field                   | Access | Type                        |
|-------------------------|--------|-----------------------------|
| [data](#data)           | (+?)   | `void *`                    |
| [elemsize](#elemsize)   | (-)    | `size_t`                    |
| [size](#size)           | (-)    | `size_t`                    |
| [capacity](#capacity)   | (-)    | `size_t`                    |
| [finalizer](#finalizer) | (+?)   | `void (*finalizer)(void *)` |

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

Allocate new uninitialized data of the given `size`.

```c
void *
alloc_new(size_t size)
```

### alloc\_new0

Like [alloc_new](#alloc_new) but zero initialize the memory.

```c
void *
alloc_new0(size_t size)
```

### alloc\_array

Allocate an uninitialized array of `len` elements of `elemsize` individually.

```c
void *
alloc_array(size_t len, size_t elemsize)
```

### alloc\_array0

Like [alloc_array](#alloc_array) but zero initialize the memory.

```c
void *
alloc_array0(size_t len, size_t elemsize)
```

### alloc\_renew

Reallocate the pointer `ptr` (which may be NULL) to the new `size` size. The
size can be 0.

```c
void *
alloc_renew(void *ptr, size_t size)
```

### alloc\_rearray

Reallocate the pointer `ptr` (which may be NULL) as an array of `newlen`
elements of `elemsize` individually.

```c
void *
alloc_rearray(void *ptr, size_t newlen, size_t elemsize)
```

### alloc\_rearray0

Reallocate the `ptr` (which maybe NULL) with the `oldlen` as current number of
elements of `elemsize` to the `newlen`. If the `newlen` is greater than oldlen,
the new region is cleared with 0.

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

Duplicate the string `src`.

```c
char *
alloc_sdup(const char *src)
```

### alloc\_sdupf

Create a dynamically allocated string using printf(3) format like.

```c
char *
alloc_sdupf(const char *fmt, ...)
```

### alloc\_free

Free memory pointed by `ptr`.

!!! note
    Use this function instead of C `free()` if you have used any of the
    functions from this module.

```c
void
alloc_free(void *ptr)
```

### alloc\_pool\_init

Initialize the `pool` as an array where elements have `elemsize` size. Optional
[`finalizer`](#finalizer) argument can be passed to finalize every element when
clearing the pool.

This will effectively create a initial storage according to
[ALLOC_POOL_INIT_DEFAULT](#alloc_pool_init_default).

```c
void
alloc_pool_init(struct alloc_pool *pool, size_t elemsize, void (*finalizer)(void *))
```

### alloc\_pool\_new

Request a new slot from the `pool`.

If the current size has reached the capacity, it will be doubled in that case it
is possible that all previous pointer may be invalidated.

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

### alloc\_pool\_shrink

Shrink the `pool`'s data to the exact number of elements in the array and return
the memory region which user takes full ownership. Once returned data is no
longer needed, it must be released with [alloc\_free](#alloc_free).

The pool is emptied and must be reinitialized using
[alloc\_pool\_init](#alloc_pool_init) before reusing it.

!!! note
    It is not necessary to call [alloc\_pool\_finish](#alloc_pool_finish).

```c
void *
alloc_pool_shrink(struct alloc_pool *pool)
```

### alloc\_pool\_finish

Finalize the `pool` and all individual element if a finalizer is set.

You must call [alloc\_pool\_init](#alloc_pool_init) again before reusing it.

```c
void
alloc_pool_finish(struct alloc_pool *pool)
```

## Examples

Use an [alloc_pool](#alloc_pool) to increase an array while reading a file.

```c
/* A structure defined line by line inside a file in the form "x|y" */
struct point {
	int x;
	int y;
};

struct alloc_pool pool;
struct point *point, *points;
size_t pointsz;
int x, y;

alloc_pool_init(&pool, sizeof (*point), NULL);

/* Assume fp is a FILE pointer allocated by the user. */
while (fscanf(fp, "%d|%d\n", &x, &y) == 2) {
	/*
	 * Returned pointer can be used to fill the region but must not be
	 * permanently referenced as it can get invalidated in the next
	 * iteration.
	 */
	point = alloc_pool_new(&pool);
	point->x = x;
	point->y = y;
}

/*
 * Shrink the data into the appropriate array length. The pool can be safely
 * discarded.
 */
pointsz = pool.size;
points = alloc_pool_shrink(&pool);

for (size_t i = 0; i < pointsz; ++i)
	point_draw(&points[i]);
```

[panic]: panic.md

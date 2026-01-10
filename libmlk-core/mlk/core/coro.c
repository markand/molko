/*
 * coro.c -- coroutine support
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#include "coro.h"
#include "panic.h"

#define MCO_ALLOC(Size) mlk_alloc_new(1, (Size))
#define MCO_DEALLOC(Ptr, Size) mlk_alloc_free((Ptr))

#define MINICORO_IMPL
#include <mlk/extern/minicoro.h>

#if !defined(NDEBUG)

#define MLK_ABORT(Coro, ...)                                                    \
do {                                                                            \
        /* If no name, provide at least the pointer for easier debug. */        \
        if (!(Coro)->name)                                                      \
                fprintf(stderr, "abort: %p ", (const void *)(Coro));            \
        else                                                                    \
                fprintf(stderr, "abort: %s ", (Coro)->name);                    \
                                                                                \
        fprintf(stderr, __VA_ARGS__);                                           \
        fprintf(stderr, "\n");                                                  \
        abort();                                                                \
} while (0)

#define MLK_ASSERT_ACTIVE(Coro)                                                 \
do {                                                                            \
        if ((Coro)->mco_coro == NULL mco_status((Coro)->mco_coro) == MCO_DEAD)  \
                MLK_ABORT((Coro), "is dead");                                   \

#define MLK_ASSERT_RESUMABLE(Coro)                                              \
do {                                                                            \
        if (mco_status((Coro)->mco_coro) != MCO_SUSPENDED)                      \
                MLK_ABORT((Coro), "is not resumable");                          \
} while (0)

#define MLK_ASSERT_YIELDABLE(Coro)                                              \
do {                                                                            \
        if (mco_status((Coro)->mco_coro) != MCO_RUNNING)                        \
                MLK_ABORT((Coro), "can't yield");                               \
} while (0)

#define MLK_RESUME(Coro)                                                        \
do {                                                                            \
        MLK_ASSERT_RESUMABLE((Coro));                                           \
        mco_resume((Coro)->mco_coro);                                           \
} while (0)

#define MLK_YIELD(Coro)                                                         \
do {                                                                            \
        MLK_ASSERT_YIELDABLE((Coro));                                           \
        mco_yield((Coro)->mco_coro);                                            \
} while (0)

#define MLK_PUSH(Into, Data, Size)                                              \
do {                                                                            \
        enum mco_result rc;                                                     \
                                                                                \
        if ((rc = mco_push((Into)->mco_coro, (Data), (Size))) != MCO_SUCCESS)   \
                MLK_ABORT((Into), "storage error: %d", rc);                     \
} while (0)

#define MLK_PULL(From, Data, Size)                                              \
do {                                                                            \
        enum mco_result rc;                                                     \
                                                                                \
        if ((rc = mco_pop((From)->mco_coro, (Data), (Size))) != MCO_SUCCESS)    \
                MLK_ABORT((From), "pull: storage error: %d", rc);               \
} while (0)

#else

#define MLK_ASSERT_RESUMABLE(Coro)
#define MLK_ASSERT_YIELDABLE(Coro)

#define MLK_RESUME(Coro)                                                        \
do {                                                                            \
        mco_resume((Coro)->mco_coro);                                           \
} while (0)

#define MLK_YIELD(Coro)                                                         \
do {                                                                            \
        mco_yield(NULL);                                                        \
} while (0)

#define MLK_PUSH(Into, Data, Size)                                              \
do {                                                                            \
        mco_push((Into)->mco_coro, (Data), (Size));                             \
} while (0)

#define MLK_PULL(From, Data, Size)                                              \
do {                                                                            \
        mco_pop((From)->mco_coro, (Data), (Size));                              \
} while (0)

#endif

static void
mlk_coro_wrap_entry(struct mco_coro *self)
{
	struct mlk_coro *coro = self->user_data;

	coro->entry(coro);
}

void
mlk_coro_init(struct mlk_coro *coro)
{
	assert(coro);

	coro->mco_desc = (const struct mco_desc) {};
	coro->mco_coro = NULL;
}

struct mlk_coro *
mlk_coro_self(void)
{
	struct mco_coro *self = mco_running();

	if (!self)
		return NULL;

	return self->user_data;
}

void
mlk_coro_yield(void)
{
	struct mlk_coro *self = mlk_coro_self();

	MLK_YIELD(self);
}

int
mlk_coro_resumable(const struct mlk_coro *coro)
{
	assert(coro);

	return coro->mco_coro && mco_status(coro->mco_coro) == MCO_SUSPENDED;
}

void
mlk_coro_resume(struct mlk_coro *coro)
{
	MLK_RESUME(coro);
}

void
mlk_coro_spawn(struct mlk_coro *coro)
{
	assert(coro);
	assert(coro->entry);

	enum mco_result rc;

	coro->mco_desc = mco_desc_init(mlk_coro_wrap_entry, 0);
	coro->mco_desc.user_data = coro;

	if ((rc = mco_create(&coro->mco_coro, &coro->mco_desc)) != MCO_SUCCESS)
		mlk_panicf("mco_create: %d", rc);

	mco_resume(coro->mco_coro);
}

void
mlk_coro_return(const void *data, size_t size)
{
	assert(data);

	mlk_coro_push(mlk_coro_self(), data, size);
}

void
mlk_coro_wait(void *data, size_t size)
{
	assert(data);

	mlk_coro_pull(mlk_coro_self(), data, size);
}

void
mlk_coro_push(struct mlk_coro *into, const void *data, size_t size)
{
	assert(into);
	assert(into->mco_coro);
	assert(data);
	assert(size);

	struct mlk_coro *self = mlk_coro_self();

	while (mco_get_bytes_stored(into->mco_coro) != 0)
		MLK_YIELD(self);

	MLK_PUSH(into, data, size);

	while (mco_get_bytes_stored(into->mco_coro) != 0)
		MLK_YIELD(self);
}

void
mlk_coro_pull(struct mlk_coro *from, void *data, size_t size)
{
	assert(data);

	struct mlk_coro *self = mlk_coro_self();

	while (mco_get_bytes_stored(from->mco_coro) != size)
		MLK_YIELD(self);

	MLK_PULL(from, data, size);
}

void
mlk_coro_finish(struct mlk_coro *coro)
{
	assert(coro);

	if (!coro->mco_coro)
		return;

	mco_destroy(coro->mco_coro);
	coro->mco_coro = NULL;
	coro->mco_desc = (const struct mco_desc) {};

	if (coro->finalizer)
		coro->finalizer(coro);
}

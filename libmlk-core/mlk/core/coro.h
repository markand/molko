/*
 * coro.h -- coroutine support
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_CORO_H
#define MLK_CORE_CORO_H

/**
 * \file mlk/core/coro.h
 * \brief Coroutine support.
 *
 * This module provides coroutine support inside the library.
 *
 * Coroutines offer suspendable/resumable functions which are useful in contexts
 * where the application code needs to wait for a condition and keep the main
 * loop doing other work.
 *
 * # Performances and behavior
 *
 * Context switching can be costly and to avoid performance penalty most of the
 * functions expect the user to know the game state as no check is made in any
 * of the function.
 *
 * In other words, resuming a coroutine that is not resumable is undefined
 * behavior. However, on Debug build assertions are used to detect possible
 * programming errors.
 *
 * # Data transfer
 *
 * This module offers a minimal data transfer support through the use of the
 * functions ::mlk_coro_pull, ::mlk_coro_push and ::mlk_coro_wait,
 * ::mlk_coro_return.
 *
 * None of those functions resume any other coroutines and is up to the caller
 * to resume its coroutines in the order required.
 */

#include <mlk/extern/minicoro.h>

/**
 * \struct mlk_coro
 * \brief Coroutine object.
 *
 */
struct mlk_coro {
	/**
	 * (optional)
	 *
	 * Coroutine name used for diagnostics and debugging.
	 */
	const char *name;

	/**
	 * (init)
	 *
	 * Coroutine entrypoint.
	 *
	 * This function is called when ::mlk_coro_spawn is called and only one
	 * time.
	 */
	void (*entry)(struct mlk_coro *self);

	/**
	 * (optional)
	 *
	 * Finalizer function to be called when the coroutine is about to be
	 * destroyed.
	 *
	 * This finalizer is called with ::mlk_coro_finish and the very end of
	 * the destruction function meaning that user may free the pointer if it
	 * was dynamically allocated.
	 */
	void (*finalizer)(struct mlk_coro *self);

	/* private */
	struct mco_coro *mco_coro;
	struct mco_desc  mco_desc;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize private fields.
 */
void
mlk_coro_init(struct mlk_coro *coro);

/**
 * Return current calling coroutine.
 *
 * \return The current coroutine or NULL if caller is outside of a coroutine.
 */
struct mlk_coro *
mlk_coro_self(void);

/**
 * Yield current coroutine.
 */
void
mlk_coro_yield(void);

/**
 * Indicate if coroutine is resumable.
 *
 * \return true if resumable
 */
int
mlk_coro_resumable(const struct mlk_coro *coro);

/**
 * Resume the coroutine.
 *
 * \pre The coroutine must be resumable.
 */
void
mlk_coro_resume(struct mlk_coro *coro);

/**
 * Create and resume the coroutine immediately.
 */
void
mlk_coro_spawn(struct mlk_coro *coro);

/**
 * This function is an alias to ::mlk_coro_push using current coroutine.
 *
 * \sa ::mlk_coro_push
 */
void
mlk_coro_return(const void *data, size_t size);

/**
 * This function is similar to ::mlk_coro_pull except that it yields the calling
 * coroutine until the data becomes available.
 *
 * \sa ::mlk_coro_pull
 */
void
mlk_coro_wait(void *data, size_t size);

/**
 * Push data into coroutine internal storage then yield until it consumes the
 * data through ::mlk_coro_wait or ::mlk_coro_pull.
 *
 * This function does not resume the coroutine passed as argument.
 *
 * \pre data != NULL
 * \param into the target coroutine
 * \param data data to push
 * \param size size of data to push
 */
void
mlk_coro_push(struct mlk_coro *into, const void *data, size_t size);

/**
 * This function is analogous to ::mlk_coro_push.
 *
 * It will first check if data is available and return immediately.
 *
 * Otherwise, it will yield until the data becomes available.
 *
 * Because this function yields forever until the data is available, caller is
 * responsible of resuming both pulling and pushing coroutines in its own loop.
 *
 * \pre data != NULL
 * \param from the coroutine to pull data from
 * \param data location where to pull data
 * \param size size of data to pull
 */
void
mlk_coro_pull(struct mlk_coro *from, void *data, size_t size);

/**
 * Destroy the coroutine and cleanup internal resources.
 *
 * No-op if already destroyed.
 *
 * \pre The coroutine must not be active.
 */
void
mlk_coro_finish(struct mlk_coro *coro);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_CORO_H */

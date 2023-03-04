/*
 * drawable-stack.h -- convenient stack of drawable objects
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

#ifndef MLK_CORE_DRAWABLE_STACK_H
#define MLK_CORE_DRAWABLE_STACK_H

/**
 * \file mlk/core/drawable-stack.h
 * \brief Convenient stack of drawable objects
 */

#include <stddef.h>

/**
 * \struct mlk_drawable_stack
 * \brief Drawable stack structure
 */
struct mlk_drawable_stack {
	/**
	 * (read-write, borrowed)
	 *
	 * Array of non-owning drawables to draw.
	 */
	struct mlk_drawable **objects;

	/**
	 * (read-write)
	 *
	 * Number of drawables in array ::mlk_drawable_stack::objects.
	 *
	 * \warning changing this value must be kept in sync with the array
	 *          dimension.
	 */
	size_t objectsz;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the drawable stack.
 *
 * This function will set all pointers in the ::mlk_drawable_stack::objects to
 * NULL.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 */
void
mlk_drawable_stack_init(struct mlk_drawable_stack *stack);

/**
 * Try to append a new drawable into the stack if one slot in the
 * ::mlk_drawable_stack::objects is NULL
 *
 * The object is inserted as-is and ownership is left to the caller.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 * \param drawable the object to append
 * \return 0 on success or ::MLK_ERR_NO_MEM if full.
 */
int
mlk_drawable_stack_add(struct mlk_drawable_stack *stack,
                       struct mlk_drawable *drawable);

/**
 * Tells if there are still at least one drawable in the stack.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 */
int
mlk_drawable_stack_completed(const struct mlk_drawable_stack *stack);

/**
 * Invoke ::mlk_draw_update on all drawables.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 * \param ticks frame ticks
 * \return non-zero if completed
 */
int
mlk_drawable_stack_update(struct mlk_drawable_stack *stack, unsigned int ticks);

/**
 * Invoke ::mlk_draw_update on all drawables.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 */
void
mlk_drawable_stack_draw(struct mlk_drawable_stack *stack);

/**
 * Invoke ::mlk_draw_finish on all drawables left.
 *
 * \pre stack != NULL
 * \param stack the drawable stack
 */
void
mlk_drawable_stack_finish(struct mlk_drawable_stack *stack);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_DRAWABLE_STACK_H */

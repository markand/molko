/*
 * drawable.h -- automatic drawable objects
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_DRAWABLE_H
#define MOLKO_DRAWABLE_H

/**
 * \file drawable.h
 * \brief Automatic drawable objects.
 */

#include <stdbool.h>

/**
 * \brief Maximum number of drawable object into a stack.
 */
#define DRAWABLE_STACK_MAX      128

/**
 * \brief Abstract drawable object.
 *
 * This structure is used to
 */
struct drawable {
	void *data;     /*!< (RW) Drawable data. */
	int x;          /*!< (RW) X coordinate if necessary. */
	int y;          /*!< (RW) Y coordinate if necessary. */

	/**
	 * Update this drawable.
	 *
	 * \param dw the drawable object
	 * \param ticks the number of ticks since last frame
	 * \return true if object has ended
	 */
	bool (*update)(struct drawable *dw, unsigned int ticks);

	/**
	 * Draw this drawable.
	 *
	 * \param dw the drawable object
	 */
	void (*draw)(struct drawable *dw);

	/**
	 * (RW)
	 *
	 * Called immediately after the drawable ended.
	 *
	 * \param act this action
	 */
	void (*end)(struct drawable *act);
	
	/**
	 * Destroy the drawable if necessary.
	 *
	 * \note This function is optional and can be NULL.
	 * \param dw the drawable object
	 */
	void (*finish)(struct drawable *dw);
};

/**
 * Shortcut for dw->update (if not NULL).
 *
 * \pre dw != NULL
 * \param dw the drawable object
 * \param ticks elapsed milliseconds since last frame
 * \return true if the drawable ended
 */
bool
drawable_update(struct drawable *dw, unsigned int ticks);

/**
 * Shortcut for dw->draw (if not NULL).
 *
 * \pre dw != NULL
 * \param dw the drawable object
 */
void
drawable_draw(struct drawable *dw);

/**
 * Shortcut for dw->end (if not NULL).
 *
 * \pre dw != NULL
 * \param dw the drawable object
 */
void
drawable_end(struct drawable *dw);

/**
 * Shortcut for dw->finish (if not NULL).
 *
 * \pre dw != NULL
 * \param dw the drawable object
 */
void
drawable_finish(struct drawable *dw);

/**
 * \brief Stack of drawable objects.
 *
 * This stack of drawable object can be used to store drawable objects within
 * a specific transition (state, battle, menu, etc).
 *
 * You can add, clear, update and draw them.
 */
struct drawable_stack {
	struct drawable *objects[DRAWABLE_STACK_MAX];   /*!< (RW) Drawables. */
};

/**
 * Initialize the stack.
 *
 * \pre st != NULL
 * \param st the drawable stack
 */
void
drawable_stack_init(struct drawable_stack *st);

/**
 * Add a drawable object into the stack.
 *
 * The drawable object must be kept alive until the stack uses it.
 *
 * \pre st != NULL
 * \pre dw != NULL
 * \param st the stack
 * \param dw the drawable to reference
 * \return true if the drawable was placed correctly and false if there wasn't
 *         enough room.
 */
bool
drawable_stack_add(struct drawable_stack *st, struct drawable *dw);

/**
 * Update all drawable objects.
 *
 * Also remove drawable objects if they were finished.
 *
 * \pre st != NULL
 * \param st the drawable stack
 * \param ticks the number of ticks since last frame
 * \return true if all drawable were rendered
 */
bool
drawable_stack_update(struct drawable_stack *st, unsigned int ticks);

/**
 * Draw all drawable objects.
 *
 * \pre st != NULL
 * \param st the drawable stack
 */
void
drawable_stack_draw(struct drawable_stack *st);

/**
 * Tells if there is any pending drawable in the stack.
 *
 * \pre st != NULL
 * \param st the stack
 * \return false if there is at least one drawable in the stack
 */
bool
drawable_stack_completed(const struct drawable_stack *st);

/**
 * Clear all drawable objects into the stack.
 */
void
drawable_stack_finish(struct drawable_stack *st);

#endif /* !MOLKO_DRAWABLE_H */

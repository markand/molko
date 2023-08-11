/*
 * checkbox.h -- GUI checkbox
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

#ifndef MLK_UI_CHECKBOX_H
#define MLK_UI_CHECKBOX_H

/**
 * \file mlk/ui/checkbox.h
 * \brief GUI checkbox.
 */

struct mlk_checkbox;
struct mlk_checkbox_delegate;
struct mlk_style;

union mlk_event;

/**
 * \struct mlk_checkbox
 * \brief UI clickable checkbox.
 */
struct mlk_checkbox {
	/**
	 * (read-write)
	 *
	 * Position in x.
	 */
	int x;

	/**
	 * (read-write)
	 *
	 * Position in y.
	 */
	int y;

	/**
	 * (read-write)
	 *
	 * Checkbox width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Checkbox height.
	 */
	unsigned int h;

	/**
	 * (read-write)
	 *
	 * Checkbox status, non-zero if activated.
	 */
	int checked;

	/**
	 * (read-write, borrowed)
	 *
	 * Checkbox delegate.
	 */
	struct mlk_checkbox_delegate *delegate;

	/**
	 * (read-write, borrowed)
	 *
	 * Checkbox style.
	 */
	struct mlk_style *style;
};

/**
 * \struct mlk_checkbox_delegate
 * \brief Checkbox delegate.
 */
struct mlk_checkbox_delegate {
	/*
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Handle an event.
	 *
	 * \param self this delegate
	 * \param cb the checkbox
	 * \param ev the event
	 * \return the current checkbox status (1 or 0)
	 */
	int (*handle)(struct mlk_checkbox_delegate *self,
	              struct mlk_checkbox *cb,
	              const union mlk_event *ev);

	/**
	 * (read-write, optional)
	 *
	 * Update the checkbox.
	 *
	 * \param self this delegate
	 * \param cb the checkbox to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_checkbox_delegate *self,
	               struct mlk_checkbox *cb,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this checkbox.
	 *
	 * \param self this delegate
	 * \param cb the checkbox to update
	 */
	void (*draw)(struct mlk_checkbox_delegate *self,
	             const struct mlk_checkbox *cb);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup this delegate associated with the checkbox.
	 *
	 * \param self this delegate
	 * \param cb the underlying checkbox
	 */
	void (*finish)(struct mlk_checkbox_delegate *self,
	               struct mlk_checkbox *cb);
};

/**
 * \brief Default stateless delegate for checkbox.
 */
extern struct mlk_checkbox_delegate mlk_checkbox_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the checkbox with default values.
 *
 * This is not required if you use designated initializers.
 *
 * \pre cb != NULL
 * \param cb the checkbox to default initialize
 * \param st style to use (or NULL to use a default)
 * \param dt delegate to use (or NULL to use a default)
 */
void
mlk_checkbox_init(struct mlk_checkbox *cb,
                  struct mlk_checkbox_delegate *dt,
                  struct mlk_style *st);

/**
 * Invoke ::mlk_checkbox_delegate::update.
 */
int
mlk_checkbox_handle(struct mlk_checkbox *cb, const union mlk_event *ev);

/**
 * Invoke ::mlk_checkbox_delegate::update.
 */
void
mlk_checkbox_update(struct mlk_checkbox *cb, unsigned int ticks);

/**
 * Invoke ::mlk_checkbox_delegate::draw.
 */
void
mlk_checkbox_draw(const struct mlk_checkbox *cb);

/**
 * Invoke ::mlk_checkbox_delegate::finish.
 */
void
mlk_checkbox_finish(struct mlk_checkbox *cb);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_CHECKBOX_H */

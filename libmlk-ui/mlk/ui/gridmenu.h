/*
 * gridmenu.h -- GUI grid menu
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

#ifndef MLK_UI_GRIDMENU_H
#define MLK_UI_GRIDMENU_H

#include <stddef.h>

struct mlk_gridmenu;
struct mlk_gridmenu_delegate;
struct mlk_style;

union mlk_event;

struct mlk_gridmenu {
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
	 * Menu frame width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Menu frame height.
	 */
	unsigned int h;

	/**
	 * (read-write, borrowed)
	 *
	 * List of strings to show.
	 */
	const char * const *items;

	/**
	 * (read-write)
	 *
	 * Number of items in ::mlk_gridmenu::items.
	 */
	size_t itemsz;

	/**
	 * (read-write)
	 *
	 * Current selected item in the grid.
	 */
	size_t selected;

	/**
	 * (read-write)
	 *
	 * Number of rows to be shown within the frame height.
	 */
	unsigned int nrows;

	/**
	 * (read-write)
	 *
	 * Number of colums to be shown within the frame width.
	 */
	unsigned int ncols;

	/**
	 * (read-write, borrowed)
	 *
	 * Grid menu delegate.
	 */
	struct mlk_gridmenu_delegate *delegate;

	/**
	 * (read-write, borrowed)
	 *
	 * Grid menu style.
	 */
	struct mlk_style *style;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int eltw;      /* maximum entry label width */
	unsigned int elth;      /* maximum entry label height */
	unsigned int spacew;    /* space between element horizontally */
	unsigned int spaceh;    /* and vertically */
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \struct mlk_gridmenu_delegate
 * \brief Grid menu delegate.
 */
struct mlk_gridmenu_delegate {
	/*
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Called after resizing the grid menu dimensions.
	 *
	 * \param self this delegate
	 * \param menu the grid menu
	 */
	void (*resize)(struct mlk_gridmenu_delegate *self,
	               struct mlk_gridmenu *menu);

	/**
	 * (read-write, optional)
	 *
	 * Handle an event.
	 *
	 * \param self this delegate
	 * \param menu the menu
	 * \param ev the event
	 * \return non-zero if an item has been selected
	 */
	int (*handle)(struct mlk_gridmenu_delegate *self,
	              struct mlk_gridmenu *menu,
	              const union mlk_event *ev);

	/**
	 * (read-write, optional)
	 *
	 * Update the grid menu.
	 *
	 * \param self this delegate
	 * \param menu the menu to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_gridmenu_delegate *self,
	               struct mlk_gridmenu *menu,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this menu.
	 *
	 * \param self this delegate
	 * \param menu the menu to draw
	 */
	void (*draw)(struct mlk_gridmenu_delegate *self,
	             const struct mlk_gridmenu *menu);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup this delegate associated with the menu.
	 *
	 * \param self this delegate
	 * \param frame the underlying frame
	 */
	void (*finish)(struct mlk_gridmenu_delegate *self,
	               struct mlk_gridmenu *menu);

};

extern struct mlk_gridmenu_delegate mlk_gridmenu_delegate;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the menu with default values.
 *
 * This is not required if you use designated initializers.
 *
 * \pre menu != NULL
 * \param menu the grid menu to default initialize
 * \param st style to use (or NULL to use a default)
 * \param dt delegate to use (or NULL to use a default)
 */
void
mlk_gridmenu_init(struct mlk_gridmenu *menu,
                  struct mlk_gridmenu_delegate *dt,
                  struct mlk_style *st);

/**
 * Copy new dimensions and then invoke ::mlk_gridmenu_delegate::resize.
 *
 * \pre menu != NULL
 * \param menu the menu to resize
 * \param x new position in x
 * \param y new position in y
 * \param w new width
 * \param h new height
 */
void
mlk_gridmenu_resize(struct mlk_gridmenu *menu,
                    int x,
                    int y,
                    unsigned int w,
                    unsigned int h);

/**
 * Invoke ::mlk_gridmenu_delegate::handle.
 */
int
mlk_gridmenu_handle(struct mlk_gridmenu *menu, const union mlk_event *ev);

/**
 * Invoke ::mlk_gridmenu_delegate::update.
 */
void
mlk_gridmenu_update(struct mlk_gridmenu *menu, unsigned int ticks);

/**
 * Invoke ::mlk_gridmenu_delegate::draw.
 */
void
mlk_gridmenu_draw(const struct mlk_gridmenu *menu);

/**
 * Invoke ::mlk_gridmenu_delegate::finish.
 */
void
mlk_gridmenu_finish(struct mlk_gridmenu *menu);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_GRIDMENU_H */

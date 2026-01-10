/*
 * gridmenu.h -- GUI grid menu
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

#ifndef MLK_UI_GRIDMENU_H
#define MLK_UI_GRIDMENU_H

#include <stddef.h>

struct mlk_font;
struct mlk_gridmenu;
struct mlk_gridmenu_style;

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
	 * Style to use for drawing this menu.
	 */
	struct mlk_gridmenu_style *style;

	/** \cond MLK_PRIVATE_DECLS */
	unsigned int eltw;      /* maximum entry label width */
	unsigned int elth;      /* maximum entry label height */
	unsigned int spacew;    /* space between element horizontally */
	unsigned int spaceh;    /* and vertically */
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \struct mlk_gridmenu_style
 * \brief Grid menu style.
 */
struct mlk_gridmenu_style {
	/**
	 * (read-write)
	 *
	 * Background color.
	 */
	unsigned long background;

	/**
	 * (read-write)
	 *
	 * Border color.
	 */
	unsigned long border;

	/**
	 * (read-write)
	 *
	 * Border size.
	 */
	unsigned int border_size;

	/**
	 * (read-write)
	 *
	 * Text color.
	 */
	unsigned long color;

	/**
	 * (read-write)
	 *
	 * Selected color.
	 */
	unsigned long color_selected;

	/**
	 * (read-write)
	 *
	 * Padding around items.
	 */
	unsigned int padding;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Font for drawing text.
	 */
	struct mlk_font *font;

	/*
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write, optional)
	 *
	 * Update the grid menu.
	 *
	 * \param self this style
	 * \param menu the menu to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_gridmenu_style *self,
	               struct mlk_gridmenu *menu,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw a frame box for this menu.
	 *
	 * \param self this style
	 * \param menu the underlying menu
	 */
	void (*draw_frame)(struct mlk_gridmenu_style *self,
	                   struct mlk_gridmenu *menu);

	/**
	 * (read-write, optional)
	 *
	 * Draw a specific item.
	 *
	 * \param self this style
	 * \param menu the underlying menu
	 * \param item the item content
	 * \param row the row number (relative)
	 * \param col the column number (relative)
	 * \param selected non-zero if the item is currently selected
	 */
	void (*draw_text)(struct mlk_gridmenu_style *self,
	                  struct mlk_gridmenu *menu,
	                  const char *item,
	                  unsigned int row,
	                  unsigned int col,
	                  int selected);
};

/**
 * \brief Dark default style for gridmenu.
 */
extern struct mlk_gridmenu_style mlk_gridmenu_style_dark;

/**
 * \brief Light default style for gridmenu.
 */
extern struct mlk_gridmenu_style mlk_gridmenu_style_light;

/**
 * \brief Default style for all gridmenus.
 */
extern struct mlk_gridmenu_style *mlk_gridmenu_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Prepare the grid menu before drawing.
 *
 * This function should be called at least once and/or when the elements are
 * updated, the geometry or the style changes.
 *
 * \pre menu != NULL
 * \param menu the menu
 */
void
mlk_gridmenu_start(struct mlk_gridmenu *menu);

/**
 * Handle an event.
 *
 * \pre menu != NULL
 * \pre ev != NULL
 * \param menu the menu
 * \param ev the event
 * \return non-zero if a new item is selected, 0 otherwise
 */
int
mlk_gridmenu_handle(struct mlk_gridmenu *menu, const union mlk_event *ev);

/**
 * Invoke ::mlk_gridmenu_style::update.
 */
void
mlk_gridmenu_update(struct mlk_gridmenu *menu, unsigned int ticks);

/**
 * Invoke the functions:
 *
 * - ::mlk_gridmenu_style::draw_frame,
 * - ::mlk_gridmenu_style::draw_item.
 */
void
mlk_gridmenu_draw(struct mlk_gridmenu *menu);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_GRIDMENU_H */

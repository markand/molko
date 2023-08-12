/*
 * label.h -- GUI label
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

#ifndef MLK_UI_LABEL_H
#define MLK_UI_LABEL_H

/**
 * \file label.h
 * \brief GUI label.
 */

struct mlk_label;
struct mlk_label_if;
struct mlk_style;

/**
 * \struct mlk_label
 * \brief UI label text.
 */
struct mlk_label {
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
	 * (read-write, borrowed)
	 *
	 * Text to show.
	 */
	const char *text;

	/**
	 * (read-write)
	 *
	 * Tells if label is considered selected.
	 */
	int selected;

	/**
	 * (read-write, borrowed)
	 *
	 * Label style.
	 */
	struct mlk_style *style;

	/**
	 * (read-write, borrowed)
	 *
	 * Label interface.
	 */
	struct mlk_label_if *iface;
};

/**
 * \struct mlk_label_if
 * \brief Label interface.
 */
struct mlk_label_if {
	/*
	 * (read-write, borrowed, optional)
	 *
	 * Arbitrary user data.
	 */
	void *data;

	/**
	 * (read-write)
	 *
	 * Query required dimensions to draw this label.
	 *
	 * \param self this interface
	 * \param lbl the label to query
	 * \param w the width destination (maybe NULL)
	 * \param h the height destination (maybe NULL)
	 * \return 0 on success or -1 on error
	 */
	int (*query)(struct mlk_label_if *self,
	             const struct mlk_label *lbl,
	             unsigned int *w,
	             unsigned int *h);

	/**
	 * (read-write, optional)
	 *
	 * Update the label.
	 *
	 * \param self this delegate
	 * \param lbl the label to update
	 * \param ticks number of ticks since last frame
	 */
	void (*update)(struct mlk_label_if *self,
	               struct mlk_label *lbl,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this label.
	 *
	 * \param self this interface
	 * \param lbl the label to update
	 */
	void (*draw)(struct mlk_label_if *self,
	             const struct mlk_label *lbl);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup data for this label.
	 *
	 * \param self this interface
	 * \param lbl the underlying label
	 */
	void (*finish)(struct mlk_label_if *self,
	               struct mlk_label *lbl);
};

/**
 * \brief Default stateless interface for label.
 */
extern struct mlk_label_if mlk_label_if;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize the label with default values.
 *
 * This is not required if you use designated initializers.
 *
 * \pre lbl != NULL
 * \param lbl the label to default initialize
 * \param dt interface to use (or NULL to use a default)
 * \param st style to use (or NULL to use a default)
 */
void
mlk_label_init(struct mlk_label *lbl,
               struct mlk_label_if *dt,
	       struct mlk_style *st);

/**
 * Invoke ::mlk_label_if::query.
 */
int
mlk_label_query(const struct mlk_label *lbl, unsigned int *w, unsigned int *h);

/**
 * Invoke ::mlk_label_if::update.
 */
void
mlk_label_update(struct mlk_label *lbl, unsigned int ticks);

/**
 * Invoke ::mlk_label_if::draw.
 */
void
mlk_label_draw(const struct mlk_label *lbl);

/**
 * Invoke ::mlk_label_if::finish.
 */
void
mlk_label_finish(struct mlk_label *lbl);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_LABEL_H */

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
struct mlk_label_delegate;
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
	 * (read-write, borrowed)
	 *
	 * Label style.
	 */
	struct mlk_style *style;

	/**
	 * (read-write, borrowed)
	 *
	 * Label delegate.
	 */
	struct mlk_label_delegate *delegate;
};

/**
 * \struct mlk_label_delegate
 * \brief Label delegate.
 */
struct mlk_label_delegate {
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
	 * \param self this delegate
	 * \param lbl the label to query
	 * \param w the width destination (maybe NULL)
	 * \param h the height destination (maybe NULL)
	 * \return 0 on success or -1 on error
	 */
	int (*query)(struct mlk_label_delegate *self,
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
	void (*update)(struct mlk_label_delegate *self,
	               struct mlk_label *lbl,
	               unsigned int ticks);

	/**
	 * (read-write, optional)
	 *
	 * Draw this label.
	 *
	 * \param self this delegate
	 * \param lbl the label to update
	 */
	void (*draw)(struct mlk_label_delegate *self,
	             const struct mlk_label *lbl);

	/**
	 * (read-write, optional)
	 *
	 * Cleanup this delegate associated with the label.
	 *
	 * \param self this delegate
	 * \param lbl the underlying label
	 */
	void (*finish)(struct mlk_label_delegate *self,
	               struct mlk_label *lbl);
};

/**
 * \brief Default stateless delegate for label.
 */
extern struct mlk_label_delegate mlk_label_delegate;

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
 * \param st style to use (or NULL to use a default)
 * \param dt delegate to use (or NULL to use a default)
 */
void
mlk_label_init(struct mlk_label *lbl,
               struct mlk_style *st,
               struct mlk_label_delegate *dt);

/**
 * Invoke ::mlk_label_delegate::query.
 */
int
mlk_label_query(const struct mlk_label *lbl, unsigned int *w, unsigned int *h);

/**
 * Invoke ::mlk_label_delegate::update.
 */
void
mlk_label_update(struct mlk_label *lbl, unsigned int ticks);

/**
 * Invoke ::mlk_label_delegate::draw.
 */
void
mlk_label_draw(const struct mlk_label *lbl);

/**
 * Invoke ::mlk_label_delegate::finish.
 */
void
mlk_label_finish(struct mlk_label *lbl);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_LABEL_H */

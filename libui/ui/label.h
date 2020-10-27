/*
 * label.h -- GUI label
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

#ifndef MOLKO_LABEL_H
#define MOLKO_LABEL_H

/**
 * \file label.h
 * \brief GUI label.
 * \ingroup ui
 */

#include <stdbool.h>

struct action;
struct theme;

/**
 * \brief Label flags.
 */
enum label_flags {
	LABEL_FLAGS_NONE,                       /*!< No flags. */
	LABEL_FLAGS_SHADOW      = (1 << 0),     /*!< Enable shadow. */
};

/**
 * \brief GUI label.
 */
struct label {
	int x;                          /*!< (+) Position in x. */
	int y;                          /*!< (+) Position in y. */
	const char *text;               /*!< (+&) Text to show. */
	enum label_flags flags;         /*!< (+) Optional flags. */
	const struct theme *theme;      /*!< (+&?) Theme to use. */
};

/**
 * Default drawing function.
 *
 * \pre t != NULL
 * \pre label != NULL
 * \param t the theme
 * \param label the label
 */
void
label_draw_default(const struct theme *t, const struct label *label);

/**
 * Tells if the label is usable.
 *
 * \param label the label to check (may be NULL)
 * \return False if label is null or as empty text.
 */
bool
label_ok(const struct label *label);

/**
 * Update the `w` and `h` fields with the dimensions the text would needs with
 * the current theme.
 *
 * \pre label != NULL
 * \param label the label
 * \param w the pointer to width (may be NULL)
 * \param h the pointer to height (may be NULL)
 */
void
label_query(const struct label *label, unsigned int *w, unsigned int *h);

/**
 * Draw the label.
 *
 * \pre label != NULL
 * \param label the label to draw
 */
void
label_draw(const struct label *label);

/**
 * Convert the label into an action.
 *
 * The following field will be set into the action:
 *
 * - act->data: points to label (reference),
 * - act->draw: a wrapper to label_draw.
 *
 * The label being an UI element is considered to never completes, as such
 * you will need to handle this case or to use a custom update function.
 *
 * \pre frame != NULL
 * \pre act != NULL
 * \param label the label to reference
 * \param act the action to fill
 */
void
label_action(struct label *label, struct action *act);

#endif /* !MOLKO_LABEL_H */

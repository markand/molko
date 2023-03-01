/*
 * button-style-glow.h -- example of glowing button
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

#ifndef MLK_EXAMPLE_BUTTON_STYLE_H
#define MLK_EXAMPLE_BUTTON_STYLE_H

#include <mlk/ui/button.h>

#define BUTTON_STYLE_GLOW_COLOR_1       0x7da42dff
#define BUTTON_STYLE_GLOW_COLOR_2       0xa6cc34ff
#define BUTTON_STYLE_GLOW_DELAY         20

struct button_style_glow {
	unsigned long colors[3];
	unsigned int delay;
	unsigned int elapsed;
	struct mlk_button_style style;
};

void
button_style_glow_init(struct button_style_glow *);

#endif /* !MLK_EXAMPLE_BUTTON_GLOW_STYLE_H */

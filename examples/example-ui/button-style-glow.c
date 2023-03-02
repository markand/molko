/*
 * button-style-glow.c -- example of glowing button
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

#include <assert.h>

#include <mlk/core/color.h>

#include <mlk/example/glower.h>

#include "button-style-glow.h"

/*
 * This button style illuminates the background color by periodically changing
 * its red, green, blue components to the target color and then go back to the
 * original transition.
 *
 * Its sole purpose is to demonstrates how style works. Because the style needs
 * a state it is implemented through a parent button_style_glow structure which
 * holds an underlying style.
 *
 * This style does not override the glow function because we just change the
 * bg_color property that will be reused.
 */

static void
init(struct mlk_button_style *style, struct mlk_button *button)
{
	(void)button;

	struct button_style_glow *styler = style->data;

	styler->glow->color = &style->bg_color;

	mlk_glower_init(styler->glow);
}

static void
update(struct mlk_button_style *style, struct mlk_button *button, unsigned int ticks)
{
	(void)button;

	struct button_style_glow *styler = style->data;

	mlk_glower_update(styler->glow, ticks);
}

void
button_style_glow_init(struct button_style_glow *styler)
{
	assert(styler);
	assert(styler->glow);

	styler->style.data = styler;
	styler->style.init = init;
	styler->style.update = update;
}

/*
 * button-style-glow.c -- example of glowing button
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

#include <assert.h>

#include <mlk/core/util.h>

#include "button-glower.h"

#define BUTTON_GLOWER(Ptr, Field) \
        (MLK_UTIL_CONTAINER_OF(Ptr, struct button_glower, Field))

static void
update(struct mlk_button_style *self, struct mlk_button *button, unsigned int ticks)
{
	struct button_glower *glower = BUTTON_GLOWER(self, style);

	/* Don't update if pressed. */
	if (!button->pressed) {
		mlk_glower_update(&glower->glower, ticks);
		glower->style.background = glower->glower.color;
	}
}

void
button_glower_init(struct button_glower *glower, struct mlk_button *button)
{
	assert(glower);
	assert(button);

	glower->style.background = glower->glower.start;
	glower->style.update = update;

	button->style = &glower->style;

	mlk_glower_init(&glower->glower);
}

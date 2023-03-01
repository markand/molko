/*
 * ui.c -- libui convenient header
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

#include <stddef.h>

#include <mlk/core/font.h>
#include <mlk/core/util.h>

#include <assets/fonts/opensans-regular.h>

#include "ui.h"

/* Default font catalog. */
#define FONT_DEF(b, s, i)                                               \
{                                                                       \
        .data = b,                                                      \
        .datasz = sizeof (b),                                           \
        .size = s,                                                      \
        .fontaddr = &mlk_ui_fonts[i]                                    \
}

static struct font_def {
	const unsigned char *data;
	const size_t datasz;
	unsigned int size;
	struct mlk_font **fontaddr;
	struct mlk_font font;
} fonts[MLK_UI_FONT_LAST] = {
	FONT_DEF(assets_fonts_opensans_regular, 14, MLK_UI_FONT_INTERFACE)
};

struct mlk_font *mlk_ui_fonts[MLK_UI_FONT_LAST] = {0};

int
mlk_ui_init(void)
{
	struct font_def *def;
	int err;

	/* Open all fonts and set the appropriate pointer address. */
	for (size_t i = 0; i < MLK_UTIL_SIZE(fonts); ++i) {
		def = &fonts[i];

		if ((err = mlk_font_openmem(&def->font, def->data, def->datasz, def->size)) < 0)
			goto failed;

		/* Reference this font into the catalog. */
		*fonts[i].fontaddr = &fonts[i].font;
	}

	return 0;

failed:
	mlk_ui_finish();

	return err;
}

void
mlk_ui_finish(void)
{
}

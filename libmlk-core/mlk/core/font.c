/*
 * font.c -- basic font management
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
#include <string.h>

#include <SDL_ttf.h>

#include "color.h"
#include "err.h"
#include "font.h"
#include "texture_p.h"
#include "util.h"

int
mlk_font_open(struct mlk_font *font, const char *path, unsigned int size)
{
	assert(font);
	assert(path);

	if (!(font->handle = TTF_OpenFont(path, size)))
		return mlk_errf("%s", SDL_GetError());

	return 0;
}

int
mlk_font_openmem(struct mlk_font *font,
                 const void *buffer,
                 size_t buflen,
                 unsigned int size)
{
	assert(font);
	assert(buffer);

	SDL_RWops *ops;

	if (!(ops = SDL_RWFromConstMem(buffer, buflen)) ||
	   (!(font->handle = TTF_OpenFontRW(ops, 1, size))))
		return mlk_errf("%s", SDL_GetError());

	return 0;
}

int
mlk_font_render(struct mlk_font *font, struct mlk_texture *tex, const char *text, unsigned long color)
{
	assert(font);
	assert(tex);
	assert(text);

	SDL_Color fg = {
		.r = MLK_COLOR_R(color),
		.g = MLK_COLOR_G(color),
		.b = MLK_COLOR_B(color),
		.a = MLK_COLOR_A(color)
	};
	SDL_Surface *surface;
	SDL_Surface *(*func)(TTF_Font *, const char *, SDL_Color);

	switch (font->style) {
	case MLK_FONT_STYLE_ANTIALIASED:
		func = TTF_RenderUTF8_Blended;
		break;
	default:
		func = TTF_RenderUTF8_Solid;
		break;
	}

	if (!(surface = func(font->handle, text, fg)))
		return mlk_errf("%s", SDL_GetError());

	return mlk__texture_from_surface(tex, surface);
}

unsigned int
mlk_font_height(const struct mlk_font *font)
{
	assert(font);

	return TTF_FontHeight(font->handle);
}

int
mlk_font_query(const struct mlk_font *font, const char *text, unsigned int *w, unsigned int *h)
{
	assert(font);
	assert(text);

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	if (TTF_SizeUTF8(font->handle, text, (int *)w, (int *)h) != 0)
		return mlk_errf("%s", SDL_GetError());

	return 0;
}

void
mlk_font_finish(struct mlk_font *font)
{
	assert(font);

	if (font->handle) {
		TTF_CloseFont(font->handle);
		font->handle = NULL;
	}
}

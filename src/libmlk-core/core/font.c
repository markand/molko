/*
 * font.c -- basic font management
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include "error.h"
#include "font.h"
#include "texture_p.h"
#include "util.h"

int
font_open(struct font *font, const char *path, unsigned int size)
{
	assert(font);
	assert(path);

	if (!(font->handle = TTF_OpenFont(path, size)))
		return ERR_INTERNAL;

	return 0;
}

int
font_openmem(struct font *font, const void *buffer, size_t buflen, unsigned int size)
{
	assert(font);
	assert(buffer);

	SDL_RWops *ops;

	if (!(ops = SDL_RWFromConstMem(buffer, buflen)) ||
	   (!(font->handle = TTF_OpenFontRW(ops, 1, size))))
		return ERR_INTERNAL;

	return 0;
}

int
font_ok(const struct font *font)
{
	return font && font->handle;
}

int
font_render(struct font *font, struct texture *tex, const char *text, unsigned long color)
{
	assert(font_ok(font));
	assert(text);

	SDL_Color fg = {
		.r = COLOR_R(color),
		.g = COLOR_G(color),
		.b = COLOR_B(color),
		.a = COLOR_A(color)
	};
	SDL_Surface *surface;
	SDL_Surface *(*func)(TTF_Font *, const char *, SDL_Color);

	switch (font->style) {
	case FONT_STYLE_ANTIALIASED:
		func = TTF_RenderUTF8_Blended;
		break;
	default:
		func = TTF_RenderUTF8_Solid;
		break;
	}

	if (!(surface = func(font->handle, text, fg)))
		return ERR_INTERNAL;

	return texture_from_surface(tex, surface);
}

unsigned int
font_height(const struct font *font)
{
	assert(font_ok(font));

	return TTF_FontHeight(font->handle);
}

int
font_query(const struct font *font, const char *text, unsigned int *w, unsigned int *h)
{
	assert(font_ok(font));
	assert(text);

	if (w)
		*w = 0;
	if (h)
		*h = 0;

	if (TTF_SizeUTF8(font->handle, text, (int *)w, (int *)h) != 0)
		return ERR_INTERNAL;

	return 0;
}

void
font_finish(struct font *font)
{
	assert(font);

	if (font->handle)
		TTF_CloseFont(font->handle);

	memset(font, 0, sizeof (*font));
}

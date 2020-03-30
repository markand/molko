/*
 * font.c -- basic font management
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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL_ttf.h>

#include "color.h"
#include "error.h"
#include "error_p.h"
#include "font.h"
#include "texture_p.h"
#include "util.h"

bool
font_open(struct font *font, const char *path, unsigned int size)
{
	assert(font);
	assert(path);

	if (!(font->handle = TTF_OpenFont(path, size)))
		return error_sdl();

	return true;
}

bool
font_openmem(struct font *font, const void *buffer, size_t buflen, unsigned int size)
{
	assert(font);
	assert(buffer);

	SDL_RWops *ops;

	if (!(ops = SDL_RWFromConstMem(buffer, buflen)) ||
	   (!(font->handle = TTF_OpenFontRW(ops, true, size))))
		return error_sdl();

	return true;
}

bool
font_ok(const struct font *font)
{
	assert(font);

	return font->handle;
}

bool
font_render(struct font *font, struct texture *tex, const char *text)
{
	assert(font);
	assert(text);

	SDL_Color fg = {
		.r = COLOR_R(font->color),
		.g = COLOR_G(font->color),
		.b = COLOR_B(font->color),
		.a = COLOR_A(font->color)
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
		return error_sdl();

	return texture_from_surface(tex, surface);
}

unsigned int
font_height(const struct font *font)
{
	assert(font);

	return TTF_FontHeight(font->handle);
}

void
font_finish(struct font *font)
{
	assert(font);

	if (font->handle)
		TTF_CloseFont(font->handle);
}

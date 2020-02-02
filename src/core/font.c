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

struct font {
	TTF_Font *handle;
};

struct font *
font_openf(const char *path, unsigned int size)
{
	assert(path);

	struct font *f;

	f = ecalloc(1, sizeof (struct font));

	if (!(f->handle = TTF_OpenFont(path, size))) {
		error_sdl();
		free(f);
		return NULL;
	}

	return f;
}

struct font *
font_openb(const void *buffer, size_t buflen, unsigned int size)
{
	assert(buffer);

	struct font *f;
	SDL_RWops *ops;

	f = ecalloc(1, sizeof (struct font));

	if (!(ops = SDL_RWFromConstMem(buffer, buflen)) ||
	   (!(f->handle = TTF_OpenFontRW(ops, true, size)))) {
		error_sdl();
		free(f);
		return NULL;
	}

	return f;
}

struct texture *
font_render(struct font *font, const char *text, unsigned long color)
{
	return font_render_ex(font, text, color, FONT_STYLE_ANTIALIASED);
}

struct texture *
font_render_ex(struct font *font,
               const char *text,
               unsigned long color,
               enum font_style style)
{
	assert(font);
	assert(text);

	SDL_Color fg = {
		.r = COLOR_R(color),
		.g = COLOR_G(color),
		.b = COLOR_B(color),
		.a = COLOR_A(color)
	};
	SDL_Surface *surface;
	SDL_Surface *(*func)(TTF_Font *, const char *, SDL_Color);

	switch (style) {
	case FONT_STYLE_ANTIALIASED:
		func = TTF_RenderUTF8_Blended;
		break;
	default:
		func = TTF_RenderUTF8_Solid;
		break;
	}

	if (!(surface = func(font->handle, text, fg))) {
		error_sdl();
		return NULL;
	}

	return texture_from_surface(surface);
}

unsigned int
font_height(const struct font *font)
{
	return TTF_FontHeight(font->handle);
}

void
font_close(struct font *font)
{
	assert(font);

	TTF_CloseFont(font->handle);
	free(font);
}

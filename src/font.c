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
#include "font.h"
#include "texture_p.h"

struct font {
	TTF_Font *handle;
};

struct font *
font_openf(const char *path, unsigned size)
{
	assert(path);

	struct font *f;

	if (!(f = calloc(1, sizeof (struct font))))
		return NULL;
	if (!(f->handle = TTF_OpenFont(path, size))) {
		free(f);
		return NULL;
	}

	return f;
}

struct font *
font_openb(const void *buffer, size_t buflen, unsigned size)
{
	struct font *f;
	SDL_RWops *ops;

	assert(buffer);

	if (!(f = calloc(1, sizeof (struct font))))
		return NULL;

	if (!(ops = SDL_RWFromConstMem(buffer, buflen))) {
		free(f);
		return NULL;
	}

	if (!(f->handle = TTF_OpenFontRW(ops, true, size))) {
		free(f);
		return NULL;
	}

	return NULL;
}

struct texture *
font_render(struct font *font, const char *text, uint32_t color)
{
	assert(font);
	assert(text);

	/* TODO: refactor this with window.c */
	SDL_Color fg = {
		.r = COLOR_R(color),
		.g = COLOR_G(color),
		.b = COLOR_B(color),
		.a = COLOR_A(color)
	};

	return texture_from_surface(TTF_RenderUTF8_Blended(font->handle, text, fg));
}

void
font_close(struct font *font)
{
	TTF_CloseFont(font->handle);
	free(font);
}

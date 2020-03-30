/*
 * texture.c -- basic texture management
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

#include "error.h"
#include "error_p.h"
#include "texture.h"
#include "texture_p.h"
#include "util.h"
#include "window_p.h"

bool
texture_new(struct texture *tex, unsigned int w, unsigned int h)
{
	assert(tex);

	tex->handle = SDL_CreateTexture(win.renderer,
	    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	if (!tex->handle) {
		tex->w = tex->h = 0;
		return error_sdl();
	}

	tex->w = w;
	tex->h = h;

	return true;
}

bool
texture_ok(const struct texture *tex)
{
	assert(tex);

	return tex->handle && tex->w && tex->h;
}

void
texture_draw(struct texture *tex, int x, int y)
{
	assert(tex);

	SDL_Rect dst = {
		.x = x,
		.y = y,
		.w = tex->w,
		.h = tex->h
	};

	SDL_RenderCopy(win.renderer, tex->handle, NULL, &dst);
}

void
texture_scale(struct texture *tex,
              int src_x,
              int src_y,
              unsigned src_w,
              unsigned src_h,
              int dst_x,
              int dst_y,
              unsigned dst_w,
              unsigned dst_h,
              double angle)
{
	const SDL_Rect src = {
		.x = src_x,
		.y = src_y,
		.w = src_w,
		.h = src_h
	};
	const SDL_Rect dst = {
		.x = dst_x,
		.y = dst_y,
		.w = dst_w,
		.h = dst_h
	};

	SDL_RenderCopyEx(win.renderer, tex->handle, &src, &dst, angle, NULL, SDL_FLIP_NONE);
}

void
texture_finish(struct texture *tex)
{
	assert(tex);

	if (tex->handle)
		SDL_DestroyTexture(tex->handle);

	memset(tex, 0, sizeof (*tex));
}

/* private */

bool
texture_from_surface(struct texture *tex, SDL_Surface *surface)
{
	assert(tex);
	assert(surface);

	if (!(tex->handle = SDL_CreateTextureFromSurface(win.renderer, surface))) {
		tex->w = tex->h = 0;
		return error_sdl();
	}

	tex->w = surface->w;
	tex->h = surface->h;

	return true;
}

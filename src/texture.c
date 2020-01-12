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

#include "texture.h"
#include "texture_p.h"
#include "window_p.h"

bool
texture_get_size(struct texture *tex, uint16_t *w, uint16_t *h)
{
	assert(tex);

	int rw, rh;

	if (SDL_QueryTexture(tex->handle, NULL, NULL, &rw, &rh) < 0) {
		*w = 0;
		*h = 0;
		return false;
	}

	*w = rw;
	*h = rh;

	return true;
}

void
texture_draw(struct texture *tex, int x, int y)
{
	SDL_Rect dst = {
		.x = x,
		.y = y,
		.w = 0,
		.h = 0
	};

	assert(tex);

	/* We need to determine size */
	SDL_QueryTexture(tex->handle, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(win.renderer, tex->handle, NULL, &dst);
}

void
texture_draw_ex(struct texture *tex,
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
texture_close(struct texture *tex)
{
	assert(tex);

	SDL_DestroyTexture(tex->handle);
	free(tex);
}

/* private */

struct texture *
texture_from_surface(SDL_Surface *surface)
{
	struct texture *texture;

	if (!(texture = calloc(1, sizeof (struct texture))))
		return NULL;
	if (!(texture->handle = SDL_CreateTextureFromSurface(win.renderer, surface))) {
		free(texture);
		return NULL;
	}

	return texture;
}

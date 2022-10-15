/*
 * texture.c -- basic texture management
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

#include "color.h"
#include "error.h"
#include "texture.h"
#include "texture_p.h"
#include "util.h"
#include "window.h"
#include "window_p.h"

int
texture_new(struct texture *tex, unsigned int w, unsigned int h)
{
	assert(tex);
	assert(w);
	assert(h);

	tex->handle = SDL_CreateTexture(RENDERER(),
	    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	if (!tex->handle) {
		tex->w = tex->h = 0;
		return ERR_SDL;
	}

	tex->w = w;
	tex->h = h;

	return 0;
}

int
texture_ok(const struct texture *tex)
{
	return tex && tex->handle && tex->w && tex->h;
}

int
texture_set_blend_mode(struct texture *tex, enum texture_blend blend)
{
	assert(tex);
	assert(blend >= TEXTURE_BLEND_NONE && blend <= TEXTURE_BLEND_MODULATE);

	static const SDL_BlendMode table[] = {
		[TEXTURE_BLEND_NONE] = SDL_BLENDMODE_NONE,
		[TEXTURE_BLEND_BLEND] = SDL_BLENDMODE_BLEND,
		[TEXTURE_BLEND_ADD] = SDL_BLENDMODE_ADD,
		[TEXTURE_BLEND_MODULATE] = SDL_BLENDMODE_MOD
	};

	if (SDL_SetTextureBlendMode(tex->handle, table[blend]) < 0)
		return errorf("%s", SDL_GetError());

	return 0;
}

int
texture_set_alpha_mod(struct texture *tex, unsigned int alpha)
{
	assert(texture_ok(tex));
	assert(alpha <= 255);

	if (SDL_SetTextureAlphaMod(tex->handle, alpha) < 0)
		return errorf("%s", SDL_GetError());

	return 0;
}

int
texture_set_color_mod(struct texture *tex, unsigned long color)
{
	assert(texture_ok(tex));

	if (SDL_SetTextureColorMod(tex->handle, COLOR_R(color), COLOR_G(color), COLOR_B(color)) < 0)
		return errorf("%s", SDL_GetError());

	return 0;
}

int
texture_draw(const struct texture *tex, int x, int y)
{
	assert(tex);

	SDL_Rect dst = {
		.x = x,
		.y = y,
		.w = tex->w,
		.h = tex->h
	};

	if (SDL_RenderCopy(RENDERER(), tex->handle, NULL, &dst) < 0)
		return errorf("%s", SDL_GetError());

	return 0;
}

int
texture_scale(const struct texture *tex,
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

	if (SDL_RenderCopyEx(RENDERER(), tex->handle, &src, &dst, angle, NULL, SDL_FLIP_NONE) < 0)
		return errorf("%s", SDL_GetError());

	return 0;
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

int
texture_from_surface(struct texture *tex, SDL_Surface *surface)
{
	assert(tex);
	assert(surface);

	if (!(tex->handle = SDL_CreateTextureFromSurface(RENDERER(), surface))) {
		tex->w = tex->h = 0;
		return ERR_SDL;
	}

	tex->w = surface->w;
	tex->h = surface->h;

	SDL_FreeSurface(surface);

	return 0;
}

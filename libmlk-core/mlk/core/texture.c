/*
 * texture.c -- basic texture management
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

#include "color.h"
#include "texture.h"
#include "texture_p.h"
#include "util.h"
#include "window.h"
#include "window_p.h"

int
mlk_texture_new(struct mlk_texture *tex, unsigned int w, unsigned int h)
{
	assert(tex);
	assert(w);
	assert(h);

	tex->handle = SDL_CreateTexture(MLK__RENDERER(),
	    SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	if (!tex->handle) {
		tex->w = tex->h = 0;
		return MLK_ERR_SDL;
	}

	tex->w = w;
	tex->h = h;

	return 0;
}

int
mlk_texture_ok(const struct mlk_texture *tex)
{
	return tex && tex->handle && tex->w && tex->h;
}

int
mlk_texture_set_blend_mode(struct mlk_texture *tex, enum mlk_texture_blend blend)
{
	assert(tex);
	assert(blend >= MLK_TEXTURE_BLEND_NONE && blend < MLK_TEXTURE_BLEND_LAST);

	static const SDL_BlendMode table[] = {
		[MLK_TEXTURE_BLEND_NONE] = SDL_BLENDMODE_NONE,
		[MLK_TEXTURE_BLEND_BLEND] = SDL_BLENDMODE_BLEND,
		[MLK_TEXTURE_BLEND_ADD] = SDL_BLENDMODE_ADD,
		[MLK_TEXTURE_BLEND_MODULATE] = SDL_BLENDMODE_MOD
	};

	if (SDL_SetTextureBlendMode(tex->handle, table[blend]) < 0)
		return MLK_ERR_SDL;

	return 0;
}

int
mlk_texture_set_alpha_mod(struct mlk_texture *tex, unsigned int alpha)
{
	assert(mlk_texture_ok(tex));
	assert(alpha <= 255);

	if (SDL_SetTextureAlphaMod(tex->handle, alpha) < 0)
		return MLK_ERR_SDL;

	return 0;
}

int
mlk_texture_set_color_mod(struct mlk_texture *tex, unsigned long color)
{
	assert(mlk_texture_ok(tex));

	if (SDL_SetTextureColorMod(tex->handle, MLK_COLOR_R(color), MLK_COLOR_G(color), MLK_COLOR_B(color)) < 0)
		return MLK_ERR_SDL;

	return 0;
}

int
mlk_texture_draw(const struct mlk_texture *tex, int x, int y)
{
	assert(tex);

	SDL_Rect dst = {
		.x = x,
		.y = y,
		.w = tex->w,
		.h = tex->h
	};

	if (SDL_RenderCopy(MLK__RENDERER(), tex->handle, NULL, &dst) < 0)
		return MLK_ERR_SDL;

	return 0;
}

int
mlk_texture_scale(const struct mlk_texture *tex,
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

	if (SDL_RenderCopyEx(MLK__RENDERER(), tex->handle, &src, &dst, angle, NULL, SDL_FLIP_NONE) < 0)
		return MLK_ERR_SDL;

	return 0;
}

void
mlk_texture_finish(struct mlk_texture *tex)
{
	assert(tex);

	if (tex->handle)
		SDL_DestroyTexture(tex->handle);

	memset(tex, 0, sizeof (*tex));
}

/* private */

int
mlk__texture_from_surface(struct mlk_texture *tex, SDL_Surface *surface)
{
	assert(tex);
	assert(surface);

	if (!(tex->handle = SDL_CreateTextureFromSurface(MLK__RENDERER(), surface))) {
		tex->w = tex->h = 0;
		return MLK_ERR_SDL;
	}

	tex->w = surface->w;
	tex->h = surface->h;

	SDL_FreeSurface(surface);

	return 0;
}

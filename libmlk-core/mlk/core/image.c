/*
 * image.c -- basic image management
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

#include <SDL_image.h>

#include "err.h"
#include "texture.h"
#include "vfs.h"
#include "vfs_p.h"
#include "window.h"
#include "window_p.h"

static void
dimensions(struct mlk_texture *tex)
{
	int w, h;

	if (SDL_QueryTexture(tex->handle, NULL, NULL, &w, &h) < 0)
		tex->w = tex->h = 0;
	else {
		tex->w = w;
		tex->h = h;
	}
}

int
mlk_image_open(struct mlk_texture *tex, const char *path)
{
	assert(tex);
	assert(path);

	if (!(tex->handle = IMG_LoadTexture(MLK__RENDERER(), path)))
		return mlk_errf("%s", SDL_GetError());

	dimensions(tex);

	return 0;
}

int
mlk_image_openmem(struct mlk_texture *tex, const void *buffer, size_t size)
{
	assert(tex);
	assert(buffer);

	SDL_RWops *ops = SDL_RWFromConstMem(buffer, size);

	if (!ops || !(tex->handle = IMG_LoadTexture_RW(MLK__RENDERER(), ops, 1)))
		return mlk_errf("%s", SDL_GetError());

	dimensions(tex);

	return 0;
}

int
mlk_image_openvfs(struct mlk_texture *tex, struct mlk_vfs_file *file)
{
	assert(tex);
	assert(file);

	SDL_RWops *ops;

	if (!(ops = mlk__vfs_to_rw(file)))
		return -1;
	if (!(tex->handle = IMG_LoadTexture_RW(MLK__RENDERER(), ops, 1)))
		return mlk_errf("%s", SDL_GetError());

	dimensions(tex);

	return 0;
}

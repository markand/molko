/*
 * texture.h -- basic texture management
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

#ifndef MLK_CORE_TEXTURE_H
#define MLK_CORE_TEXTURE_H

#include "core.h"
#include "err.h"

struct mlk_texture {
	unsigned int w;
	unsigned int h;
	void *handle;
};

enum mlk_texture_blend {
	MLK_TEXTURE_BLEND_NONE,
	MLK_TEXTURE_BLEND_BLEND,
	MLK_TEXTURE_BLEND_ADD,
	MLK_TEXTURE_BLEND_MODULATE,
	MLK_TEXTURE_BLEND_LAST
};

MLK_CORE_BEGIN_DECLS

int
mlk_texture_new(struct mlk_texture *, unsigned int, unsigned int);

int
mlk_texture_ok(const struct mlk_texture *);

int
mlk_texture_set_blend_mode(struct mlk_texture *, enum mlk_texture_blend);

int
mlk_texture_set_alpha_mod(struct mlk_texture *, unsigned int);

int
mlk_texture_set_color_mod(struct mlk_texture *, unsigned long);

int
mlk_texture_draw(const struct mlk_texture *, int, int);

int
mlk_texture_scale(const struct mlk_texture *,
              int,
              int,
              unsigned int,
              unsigned int,
              int,
              int,
              unsigned int,
              unsigned int,
              double);

void
mlk_texture_finish(struct mlk_texture *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_TEXTURE_H */

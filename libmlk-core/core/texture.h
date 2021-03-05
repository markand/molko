/*
 * texture.h -- basic texture management
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

#ifndef MOLKO_CORE_TEXTURE_H
#define MOLKO_CORE_TEXTURE_H

#include <stdbool.h>

#include "core.h"

struct texture {
	unsigned int w;
	unsigned int h;
	void *handle;
};

enum texture_blend {
	TEXTURE_BLEND_NONE,
	TEXTURE_BLEND_BLEND,
	TEXTURE_BLEND_ADD,
	TEXTURE_BLEND_MODULATE,
	TEXTURE_BLEND_LAST
};

CORE_BEGIN_DECLS

bool
texture_new(struct texture *tex, unsigned int w, unsigned int h);

bool
texture_ok(const struct texture *tex);

bool
texture_set_blend_mode(struct texture *tex, enum texture_blend blend);

bool
texture_set_alpha_mod(struct texture *tex, unsigned int alpha);

bool
texture_set_color_mod(struct texture *tex, unsigned long color);

bool
texture_draw(const struct texture *tex, int x, int y);

bool
texture_scale(const struct texture *tex,
              int src_x,
              int src_y,
              unsigned src_w,
              unsigned src_h,
              int dst_x,
              int dst_y,
              unsigned dst_w,
              unsigned dst_h,
              double angle);

void
texture_finish(struct texture *tex);

CORE_END_DECLS

#endif /* !MOLKO_CORE_TEXTURE_H */

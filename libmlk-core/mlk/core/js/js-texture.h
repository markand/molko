/*
 * js-texture.h -- basic texture management (Javascript bindings)
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

#ifndef MLK_CORE_JS_TEXTURE_H
#define MLK_CORE_JS_TEXTURE_H

#include <duktape.h>

struct mlk_texture;

#if defined(__cplusplus)
extern "C" {
#endif

struct mlk_texture *
mlk_js_texture_require(duk_context *ctx, duk_idx_t index);

void
mlk_js_texture_push(duk_context *ctx, const struct mlk_texture *tex);

void
mlk_js_texture_load(duk_context *ctx);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_JS_TEXTURE_H */
/*
 * js-core.c -- core binding
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include "js-core.h"

#define VFS DUK_HIDDEN_SYMBOL("Mlk.Vfs")

void
js_core_bind(duk_context *ctx, struct vfs *vfs)
{
	assert(ctx);
	assert(vfs);

	/* We put the VFS object. */
	duk_push_global_stash(ctx);
	duk_push_pointer(ctx, vfs);
	duk_put_prop_string(ctx, -2, VFS);
	duk_pop(ctx);
}

struct vfs *
js_core_global_vfs(duk_context *ctx)
{
	assert(ctx);

	struct vfs *vfs;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, VFS);
	vfs = duk_to_pointer(ctx, -1);
	duk_pop(ctx);
	
	return vfs;
}

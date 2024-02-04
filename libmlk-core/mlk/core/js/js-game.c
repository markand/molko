/*
 * js-game.c -- main game object (Javascript bindings)
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

#include <mlk/core/err.h>
#include <mlk/core/game.h>
#include <mlk/core/state.h>

#include "js-game.h"
#include "js-state.h"

#define REFS DUK_HIDDEN_SYMBOL("mlk::game::states")

static duk_ret_t
mlk_js_game_push(duk_context *ctx)
{
	struct mlk_state *st;

	st = mlk_js_state_require(ctx, 0);

	if (mlk_game_push(st) < 0)
		duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());

	/* Keep a reference to this object, otherwise it would be collected. */
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, REFS);
	duk_dup(ctx, 0);
	duk_put_prop_index(ctx, -2, duk_get_length(ctx, -2));
	duk_pop_n(ctx, 2);

	return 0;
}

static duk_ret_t
mlk_js_game_pop(duk_context *ctx)
{
	(void)ctx;

	duk_size_t len;

	mlk_game_pop();

	/* Remove reference of previous state. */
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, REFS);

	if ((len = duk_get_length(ctx, -1)))
		duk_del_prop_index(ctx, -1, len - 1);

	duk_pop_n(ctx, 2);

	return 0;
}

#if 0
static duk_ret_t
mlk_js_game_handle(duk_context *ctx)
{
}

static duk_ret_t
mlk_js_game_update(duk_context *ctx)
{
	mlk_game_update(duk_require_uint(ctx, 0));

	return 0;
}

static duk_ret_t
mlk_js_game_draw(duk_context *ctx)
{
	(void)ctx;

	mlk_game_draw();

	return 0;
}
#endif

static duk_ret_t
mlk_js_game_loop(duk_context *ctx)
{
	(void)ctx;

	mlk_game_loop(NULL);

	return 0;
}

static duk_ret_t
mlk_js_game_quit(duk_context *ctx)
{
	(void)ctx;

	mlk_game_quit();

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "push",   mlk_js_game_push,   1  },
	{ "pop",    mlk_js_game_pop,    0  },
#if 0
	{ "update", mlk_js_game_update, 1  },
	{ "draw",   mlk_js_game_draw,   0  },
#endif
	{ "loop",   mlk_js_game_loop,   0  },
	{ "quit",   mlk_js_game_quit,   0  },
	{ NULL,     NULL,               -1 },
};

void
mlk_js_game_load(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Mlk");
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_prop_string(ctx, -2, "Game");
	duk_pop(ctx);
	duk_push_global_stash(ctx);
	duk_push_array(ctx);
	duk_put_prop_string(ctx, -2, REFS);
	duk_pop(ctx);
}

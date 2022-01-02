/*
 * js-game.c -- core gamej binding
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

#include <core/game.h>
#include <core/state.h>

#include "js-game.h"
#include "js-state.h"

/*
 * TODO: determine if it's worth it to add handle, update and draw functions.
 */

static duk_ret_t
Game_push(duk_context *ctx)
{
	struct js_state *state = js_state_require(ctx, 0);

	if (game.state == &game.states[GAME_STATE_MAX]) {
		state_finish(&state->st);
		return duk_error(ctx, DUK_ERR_RANGE_ERROR, "too many states");
	}

	duk_push_this(ctx);
	state->parent = duk_get_heapptr(ctx, -1);
	duk_push_sprintf(ctx, "%p", state);
	duk_dup(ctx, 0);
	duk_put_prop(ctx, -3);
	duk_pop(ctx);

	game_push(&state->st);

	return 0;
}

static duk_ret_t
Game_pop(duk_context *ctx)
{
	(void)ctx;

	if (game.state == &game.states[0])
		return 0;

	game_pop();

	return 0;
}

static duk_ret_t
Game_loop(duk_context *ctx)
{
	(void)ctx;

	game_loop();

	return 0;
}

static duk_ret_t
Game_quit(duk_context *ctx)
{
	(void)ctx;

	game_quit();

	return 0;
}

static const duk_function_list_entry functions[] = {
	{ "push",       Game_push,      1 },
	{ "pop",        Game_pop,       0 },
	{ "loop",       Game_loop,      0 },
	{ "quit",       Game_quit,      0 },
	{ NULL,         NULL,           0 }
};

void
js_game_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_put_global_string(ctx, "Game");
}

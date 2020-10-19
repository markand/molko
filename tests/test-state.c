/*
 * test-rbuf.c -- test error routines
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

#include <string.h>

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <core/event.h>
#include <core/game.h>
#include <core/state.h>

struct invokes {
	unsigned int start;
	unsigned int handle;
	unsigned int update;
	unsigned int draw;
	unsigned int end;
	unsigned int finish;
};

static void
zero(struct invokes *inv)
{
	memset(inv, 0, sizeof (*inv));
}

static void
my_start(struct state *state)
{
	((struct invokes *)state->data)->start++;
}

static void
my_handle(struct state *state, const union event *ev)
{
	(void)ev;

	((struct invokes *)state->data)->handle++;
}

static void
my_update(struct state *state, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)state->data)->update++;
}

static void
my_draw(struct state *state)
{
	((struct invokes *)state->data)->draw++;
}

static void
my_end(struct state *state)
{
	((struct invokes *)state->data)->end++;
}

static void
my_finish(struct state *state)
{
	((struct invokes *)state->data)->finish++;
}

#define INIT(pdata) { \
	.data = pdata, \
	.start = my_start, \
	.handle = my_handle, \
	.update = my_update, \
	.draw = my_draw, \
	.end = my_end, \
	.finish = my_finish \
}

GREATEST_TEST
basics_start(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_start(&state);
	GREATEST_ASSERT_EQ(inv.start, 1);
	GREATEST_ASSERT_EQ(inv.handle, 0);
	GREATEST_ASSERT_EQ(inv.update, 0);
	GREATEST_ASSERT_EQ(inv.draw, 0);
	GREATEST_ASSERT_EQ(inv.end, 0);
	GREATEST_ASSERT_EQ(inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_handle(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_handle(&state, &(union event){0});
	GREATEST_ASSERT_EQ(inv.start, 0);
	GREATEST_ASSERT_EQ(inv.handle, 1);
	GREATEST_ASSERT_EQ(inv.update, 0);
	GREATEST_ASSERT_EQ(inv.draw, 0);
	GREATEST_ASSERT_EQ(inv.end, 0);
	GREATEST_ASSERT_EQ(inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_update(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_update(&state, 0);
	GREATEST_ASSERT_EQ(inv.start, 0);
	GREATEST_ASSERT_EQ(inv.handle, 0);
	GREATEST_ASSERT_EQ(inv.update, 1);
	GREATEST_ASSERT_EQ(inv.draw, 0);
	GREATEST_ASSERT_EQ(inv.end, 0);
	GREATEST_ASSERT_EQ(inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_draw(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_draw(&state);
	GREATEST_ASSERT_EQ(inv.start, 0);
	GREATEST_ASSERT_EQ(inv.handle, 0);
	GREATEST_ASSERT_EQ(inv.update, 0);
	GREATEST_ASSERT_EQ(inv.draw, 1);
	GREATEST_ASSERT_EQ(inv.end, 0);
	GREATEST_ASSERT_EQ(inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_end(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_end(&state);
	GREATEST_ASSERT_EQ(inv.start, 0);
	GREATEST_ASSERT_EQ(inv.handle, 0);
	GREATEST_ASSERT_EQ(inv.update, 0);
	GREATEST_ASSERT_EQ(inv.draw, 0);
	GREATEST_ASSERT_EQ(inv.end, 1);
	GREATEST_ASSERT_EQ(inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_finish(void)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_finish(&state);
	GREATEST_ASSERT_EQ(inv.start, 0);
	GREATEST_ASSERT_EQ(inv.handle, 0);
	GREATEST_ASSERT_EQ(inv.update, 0);
	GREATEST_ASSERT_EQ(inv.draw, 0);
	GREATEST_ASSERT_EQ(inv.end, 0);
	GREATEST_ASSERT_EQ(inv.finish, 1);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(basics_start);
	GREATEST_RUN_TEST(basics_handle);
	GREATEST_RUN_TEST(basics_update);
	GREATEST_RUN_TEST(basics_draw);
	GREATEST_RUN_TEST(basics_end);
	GREATEST_RUN_TEST(basics_finish);
}

static void
switch_startup(void *data)
{
	memset(data, 0, sizeof (game));
}

GREATEST_TEST
switch_quick_true(void)
{
	struct {
		struct invokes inv;
		struct state state;
	} table[2] = {
		{ .state = INIT(&table[0]) },
		{ .state = INIT(&table[1]) }
	};

	/*
	 * When set to true, switching quickly state will immediately set the
	 * current state to the specified one and call start on it. However,
	 * if there was already a planned state, it is finished immediately.
	 */
	game_switch(&table[0].state, true);

	GREATEST_ASSERT_EQ(table[0].inv.start, 1);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 0);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);

	GREATEST_ASSERT_EQ(table[1].inv.start, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);

	/* Switch from [0] to [1] quickly, [0] should be closed immediately. */
	zero(&table[0].inv);
	game_switch(&table[1].state, true);

	GREATEST_ASSERT_EQ(table[0].inv.start, 0);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 1);

	GREATEST_ASSERT_EQ(table[1].inv.start, 1);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	
	GREATEST_PASS();
}

GREATEST_TEST
switch_quick_false(void)
{
	struct {
		struct invokes inv;
		struct state state;
	} table[2] = {
		{ .state = INIT(&table[0]) },
		{ .state = INIT(&table[1]) }
	};

	game_switch(&table[0].state, true);

	GREATEST_ASSERT_EQ(table[0].inv.start, 1);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 0);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);

	GREATEST_ASSERT_EQ(table[1].inv.start, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);

	/*
	 * Switch from [0] to [1] quickly, this should do nothing as it should
	 * be done on the next game_update call instead.
	 */
	zero(&table[0].inv);
	game_switch(&table[1].state, false);

	GREATEST_ASSERT_EQ(table[0].inv.start, 0);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 0);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);

	GREATEST_ASSERT_EQ(table[1].inv.start, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	
	GREATEST_PASS();
}

GREATEST_TEST
switch_invoke(void)
{
	struct {
		struct invokes inv;
		struct state state;
	} table[2] = {
		{ .state = INIT(&table[0]) },
		{ .state = INIT(&table[1]) }
	};

	/* Start with 0. */
	game_switch(&table[0].state, true);

	/* Ask to switch to 1. */
	zero(&table[0].inv);
	game_switch(&table[1].state, true);
	game_update(0);

	GREATEST_ASSERT_EQ(table[0].inv.start, 0);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 1);

	GREATEST_ASSERT_EQ(table[1].inv.start, 1);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_switch)
{
	GREATEST_SET_SETUP_CB(switch_startup, &game);
	GREATEST_RUN_TEST(switch_quick_true);
	GREATEST_RUN_TEST(switch_quick_false);
	GREATEST_RUN_TEST(switch_invoke);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_RUN_SUITE(suite_switch);
	GREATEST_MAIN_END();
}

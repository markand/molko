/*
 * test-rbuf.c -- test error routines
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

#include <string.h>

#include <core/event.h>
#include <core/game.h>
#include <core/state.h>
#include <core/util.h>

#include "test.h"

struct invokes {
	unsigned int start;
	unsigned int handle;
	unsigned int update;
	unsigned int draw;
	unsigned int suspend;
	unsigned int resume;
	unsigned int end;
	unsigned int finish;
};

static struct state *states[16];

RX_SET_UP(basics_set_up)
{
	game_init(states, UTIL_SIZE(states));

	return RX_SUCCESS;
}

RX_TEAR_DOWN(basics_tear_down)
{
	game_quit();
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
my_suspend(struct state *state)
{
	((struct invokes *)state->data)->suspend++;
}

static void
my_resume(struct state *state)
{
	((struct invokes *)state->data)->resume++;
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
	.suspend = my_suspend, \
	.resume = my_resume, \
	.end = my_end, \
	.finish = my_finish \
}

RX_TEST_CASE(basics, start)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_start(&state);
	RX_UINT_REQUIRE_EQUAL(inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 0U);
}

RX_TEST_CASE(basics, handle)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_handle(&state, &(const union event){0});
	RX_UINT_REQUIRE_EQUAL(inv.start, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 0U);
}

RX_TEST_CASE(basics, update)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_update(&state, 0);
	RX_UINT_REQUIRE_EQUAL(inv.start, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 1U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 0U);
}

RX_TEST_CASE(basics, draw)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_draw(&state);
	RX_UINT_REQUIRE_EQUAL(inv.start, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 1U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 0U);
}

RX_TEST_CASE(basics, end)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_end(&state);
	RX_UINT_REQUIRE_EQUAL(inv.start, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 1U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 0U);
}

RX_TEST_CASE(basics, finish)
{
	struct invokes inv = {0};
	struct state state = INIT(&inv);

	state_finish(&state);
	RX_UINT_REQUIRE_EQUAL(inv.start, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(inv.finish, 1U);
}

RX_TEST_CASE(basics, game)
{
	static struct {
		struct invokes inv;
		struct state state;
	} states[] = {
		{ .state = INIT(&states[0].inv) },
		{ .state = INIT(&states[1].inv) }
	};

	/* 0 becomes active and should start. */
	game_push(&states[0].state);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 0U);

	/* Put some event, update and drawing. */
	game_handle(&(union event) { .type = EVENT_QUIT });
	game_update(100);
	game_update(100);
	game_draw();
	game_draw();
	game_draw();
	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 2U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 3U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 0U);

	/* Switch to state 1, 0 must be suspended. */
	game_push(&states[1].state);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 2U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 3U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 0U);

	RX_UINT_REQUIRE_EQUAL(states[1].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.update, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.finish, 0U);

	/* Update a little this state. */
	game_update(10);
	game_update(10);
	game_update(10);
	game_update(10);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 2U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 3U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 0U);

	RX_UINT_REQUIRE_EQUAL(states[1].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.update, 4U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.finish, 0U);

	/* Pop it, it should be finalized through end and finish. */
	game_pop();

	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 2U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 3U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 0U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 0U);

	RX_UINT_REQUIRE_EQUAL(states[1].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.update, 4U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.end, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.finish, 1U);

	/* Pop this state as well. */
	game_pop();

	RX_UINT_REQUIRE_EQUAL(states[0].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.handle, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.update, 2U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.draw, 3U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.suspend, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.resume, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.end, 1U);
	RX_UINT_REQUIRE_EQUAL(states[0].inv.finish, 1U);

	RX_UINT_REQUIRE_EQUAL(states[1].inv.start, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.handle, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.update, 4U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.draw, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.suspend, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.resume, 0U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.end, 1U);
	RX_UINT_REQUIRE_EQUAL(states[1].inv.finish, 1U);
}

static const struct rx_test_case tests[] = {
	TEST(basics, start),
	TEST(basics, handle),
	TEST(basics, update),
	TEST(basics, draw),
	TEST(basics, end),
	TEST(basics, finish),
	TEST_FIXTURE(basics, game, void *)
};

int
main(int argc, char **argv)
{
	return TEST_RUN_ALL(tests, argc, argv);
}

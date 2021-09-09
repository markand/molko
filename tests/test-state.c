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
	unsigned int suspend;
	unsigned int resume;
	unsigned int end;
	unsigned int finish;
};

static void
zero(struct invokes *inv)
{
	memset(inv, 0, sizeof (*inv));
}

static void
setup(void *data)
{
	game_init();
}

static void
cleanup(void *data)
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

GREATEST_TEST
test_game_push(void)
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
	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 0U);

	/* Put some event, update and drawing. */
	game_handle(&(union event) { .type = EVENT_QUIT });
	game_update(100);
	game_update(100);
	game_draw();
	game_draw();
	game_draw();
	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 2U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 3U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 0U);

	/* Switch to state 1, 0 must be suspended. */
	game_push(&states[1].state);
	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 2U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 3U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 0U);

	GREATEST_ASSERT_EQ(states[1].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.handle, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.update, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.draw, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.finish, 0U);

	/* Update a little this state. */
	game_update(10);
	game_update(10);
	game_update(10);
	game_update(10);
	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 2U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 3U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 0U);

	GREATEST_ASSERT_EQ(states[1].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.handle, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.update, 4U);
	GREATEST_ASSERT_EQ(states[1].inv.draw, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.finish, 0U);

	/* Pop it, it should be finalized through end and finish. */
	game_pop();

	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 2U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 3U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 0U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 0U);

	GREATEST_ASSERT_EQ(states[1].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.handle, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.update, 4U);
	GREATEST_ASSERT_EQ(states[1].inv.draw, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.end, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.finish, 1U);

	/* Pop this state as well. */
	game_pop();

	GREATEST_ASSERT_EQ(states[0].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.handle, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.update, 2U);
	GREATEST_ASSERT_EQ(states[0].inv.draw, 3U);
	GREATEST_ASSERT_EQ(states[0].inv.suspend, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.resume, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.end, 1U);
	GREATEST_ASSERT_EQ(states[0].inv.finish, 1U);

	GREATEST_ASSERT_EQ(states[1].inv.start, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.handle, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.update, 4U);
	GREATEST_ASSERT_EQ(states[1].inv.draw, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.suspend, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.resume, 0U);
	GREATEST_ASSERT_EQ(states[1].inv.end, 1U);
	GREATEST_ASSERT_EQ(states[1].inv.finish, 1U);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_game)
{
	GREATEST_SET_SETUP_CB(setup, NULL);
	GREATEST_SET_TEARDOWN_CB(cleanup, NULL);
	GREATEST_RUN_TEST(test_game_push);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_RUN_SUITE(suite_game);
	GREATEST_MAIN_END();
}

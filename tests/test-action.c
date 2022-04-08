/*
 * test-action.c -- test actions
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

#include <core/action.h>
#include <core/action-stack.h>
#include <core/event.h>

#include "test.h"

struct invokes {
	int handle;
	int update;
	int draw;
	int end;
	int finish;
};

static union event dummy;

#define INIT(dat, up) {         \
        .data = (dat),          \
        .handle = my_handle,    \
        .update = (up),         \
        .draw = my_draw,        \
        .end = my_end,          \
        .finish = my_finish     \
}

static void
my_handle(struct action *act, const union event *ev)
{
	(void)ev;

	((struct invokes *)act->data)->handle = 1;
}

static int
my_update_false(struct action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update = 1;

	return 0;
}

static int
my_update_true(struct action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update = 1;

	return 1;
}

static void
my_draw(struct action *act)
{
	((struct invokes *)act->data)->draw = 1;
}

static void
my_end(struct action *act)
{
	((struct invokes *)act->data)->end = 1;
}

static void
my_finish(struct action *act)
{
	((struct invokes *)act->data)->finish = 1;
}

RX_TEST_CASE(basics, handle)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_handle(&act, &dummy);

	RX_REQUIRE(inv.handle);
	RX_REQUIRE(!inv.update);
	RX_REQUIRE(!inv.draw);
	RX_REQUIRE(!inv.end);
	RX_REQUIRE(!inv.finish);
}

RX_TEST_CASE(basics, update)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[1], my_update_false)       }
	};

	/* True version. */
	RX_REQUIRE(action_update(&table[0].act, 0));
	RX_REQUIRE(!table[0].inv.handle);
	RX_REQUIRE(table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(!table[0].inv.end);
	RX_REQUIRE(!table[0].inv.finish);

	/* False version. */
	RX_REQUIRE(!action_update(&table[1].act, 0));
	RX_REQUIRE(!table[1].inv.handle);
	RX_REQUIRE(table[1].inv.update);
	RX_REQUIRE(!table[1].inv.draw);
	RX_REQUIRE(!table[1].inv.end);
	RX_REQUIRE(!table[1].inv.finish);
}

RX_TEST_CASE(basics, draw)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_draw(&act);

	RX_REQUIRE(!inv.handle);
	RX_REQUIRE(!inv.update);
	RX_REQUIRE(inv.draw);
	RX_REQUIRE(!inv.end);
	RX_REQUIRE(!inv.finish);
}

RX_TEST_CASE(basics, end)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_end(&act);

	RX_REQUIRE(!inv.handle);
	RX_REQUIRE(!inv.update);
	RX_REQUIRE(!inv.draw);
	RX_REQUIRE(inv.end);
	RX_REQUIRE(!inv.finish);
}

RX_TEST_CASE(basics, finish)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_finish(&act);

	RX_REQUIRE(!inv.handle);
	RX_REQUIRE(!inv.update);
	RX_REQUIRE(!inv.draw);
	RX_REQUIRE(!inv.end);
	RX_REQUIRE(inv.finish);
}

RX_TEST_CASE(stack, add)
{
	struct action *actions[10];
	struct action_stack st = {0};
	struct action act = {0};

	action_stack_init(&st, actions, 10);

	RX_INT_REQUIRE_EQUAL(action_stack_add(&st, &act), 0);

	/* Now fill up. */
	for (int i = 0; i < 9; ++i)
		RX_INT_REQUIRE_EQUAL(action_stack_add(&st, &act), 0);

	/* This one should not fit in. */
	RX_INT_REQUIRE_EQUAL(action_stack_add(&st, &act), -1);
}

RX_TEST_CASE(stack, handle)
{
	struct {
		int called;
		struct action act;
	} table[] = {
		{ 0, { .data = &table[0].called, .handle = my_handle } },
		{ 0, { .data = &table[1].called, .handle = my_handle } },
		{ 0, { .data = &table[2].called, .handle = my_handle } },
	};

	struct action *actions[10];
	struct action_stack st = {0};

	action_stack_init(&st, actions, 10);
	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_add(&st, &table[2].act);
	action_stack_handle(&st, &dummy);

	RX_REQUIRE(table[0].called);
	RX_REQUIRE(table[1].called);
	RX_REQUIRE(table[2].called);
}

RX_TEST_CASE(stack, update)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_false)       },
		{ .act = INIT(&table[1], my_update_true)        },
		{ .act = INIT(&table[2], my_update_false)       },
		{ .act = INIT(&table[3], my_update_false)       },
		{ .act = INIT(&table[4], my_update_true)        },
		{ .act = INIT(&table[5], my_update_true)        },
		{ .act = INIT(&table[6], my_update_false)	},
	};

	struct action *actions[10];
	struct action_stack st = {0};

	action_stack_init(&st, actions, 10);
	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_add(&st, &table[2].act);
	action_stack_add(&st, &table[3].act);
	action_stack_add(&st, &table[4].act);
	action_stack_add(&st, &table[5].act);
	action_stack_add(&st, &table[6].act);

	RX_REQUIRE(!action_stack_update(&st, 0));

	RX_REQUIRE(!table[0].inv.handle);
	RX_REQUIRE(!table[1].inv.handle);
	RX_REQUIRE(!table[2].inv.handle);
	RX_REQUIRE(!table[3].inv.handle);
	RX_REQUIRE(!table[4].inv.handle);
	RX_REQUIRE(!table[5].inv.handle);
	RX_REQUIRE(!table[6].inv.handle);

	RX_REQUIRE(table[0].inv.update);
	RX_REQUIRE(table[1].inv.update);
	RX_REQUIRE(table[2].inv.update);
	RX_REQUIRE(table[3].inv.update);
	RX_REQUIRE(table[4].inv.update);
	RX_REQUIRE(table[5].inv.update);
	RX_REQUIRE(table[6].inv.update);

	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(!table[1].inv.draw);
	RX_REQUIRE(!table[2].inv.draw);
	RX_REQUIRE(!table[3].inv.draw);
	RX_REQUIRE(!table[4].inv.draw);
	RX_REQUIRE(!table[5].inv.draw);
	RX_REQUIRE(!table[6].inv.draw);

	RX_REQUIRE(!table[0].inv.end);
	RX_REQUIRE(table[1].inv.end);
	RX_REQUIRE(!table[2].inv.end);
	RX_REQUIRE(!table[3].inv.end);
	RX_REQUIRE(table[4].inv.end);
	RX_REQUIRE(table[5].inv.end);
	RX_REQUIRE(!table[6].inv.end);

	RX_REQUIRE(!table[0].inv.finish);
	RX_REQUIRE(table[1].inv.finish);
	RX_REQUIRE(!table[2].inv.finish);
	RX_REQUIRE(!table[3].inv.finish);
	RX_REQUIRE(table[4].inv.finish);
	RX_REQUIRE(table[5].inv.finish);
	RX_REQUIRE(!table[6].inv.finish);

	/* The following must still be there. */
	RX_PTR_REQUIRE_EQUAL(st.actions[0], &table[0].act);
	RX_PTR_REQUIRE_EQUAL(st.actions[2], &table[2].act);
	RX_PTR_REQUIRE_EQUAL(st.actions[3], &table[3].act);
	RX_PTR_REQUIRE_EQUAL(st.actions[6], &table[6].act);

	/* The following must have been NULL-ed. */
	RX_PTR_REQUIRE_EQUAL(st.actions[1], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[4], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[5], NULL);

	/*
	 * Now make all actions to return 1 and check if it cleans the stack.
	 */
	table[0].act.update =
	    table[2].act.update =
	    table[3].act.update =
	    table[6].act.update = my_update_true;

	RX_REQUIRE(action_stack_update(&st, 0));
	RX_PTR_REQUIRE_EQUAL(st.actions[0], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[1], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[2], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[3], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[4], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[5], NULL);
	RX_PTR_REQUIRE_EQUAL(st.actions[6], NULL);
}

RX_TEST_CASE(stack, draw)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_false)       },
		{ .act = INIT(&table[1], my_update_true)        },
		{ .act = INIT(&table[2], my_update_false)       },
		{ .act = INIT(&table[3], my_update_false)       },
		{ .act = INIT(&table[4], my_update_true)        },
		{ .act = INIT(&table[5], my_update_true)        },
		{ .act = INIT(&table[6], my_update_false)	},
	};

	struct action *actions[10];
	struct action_stack st = {0};

	action_stack_init(&st, actions, 10);
	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_add(&st, &table[2].act);
	action_stack_add(&st, &table[3].act);
	action_stack_add(&st, &table[4].act);
	action_stack_add(&st, &table[5].act);
	action_stack_add(&st, &table[6].act);
	action_stack_draw(&st);

	RX_REQUIRE(!table[0].inv.handle);
	RX_REQUIRE(!table[1].inv.handle);
	RX_REQUIRE(!table[2].inv.handle);
	RX_REQUIRE(!table[3].inv.handle);
	RX_REQUIRE(!table[4].inv.handle);
	RX_REQUIRE(!table[5].inv.handle);
	RX_REQUIRE(!table[6].inv.handle);

	RX_REQUIRE(!table[0].inv.update);
	RX_REQUIRE(!table[1].inv.update);
	RX_REQUIRE(!table[2].inv.update);
	RX_REQUIRE(!table[3].inv.update);
	RX_REQUIRE(!table[4].inv.update);
	RX_REQUIRE(!table[5].inv.update);
	RX_REQUIRE(!table[6].inv.update);

	RX_REQUIRE(table[0].inv.draw);
	RX_REQUIRE(table[1].inv.draw);
	RX_REQUIRE(table[2].inv.draw);
	RX_REQUIRE(table[3].inv.draw);
	RX_REQUIRE(table[4].inv.draw);
	RX_REQUIRE(table[5].inv.draw);
	RX_REQUIRE(table[6].inv.draw);

	RX_REQUIRE(!table[0].inv.end);
	RX_REQUIRE(!table[1].inv.end);
	RX_REQUIRE(!table[2].inv.end);
	RX_REQUIRE(!table[3].inv.end);
	RX_REQUIRE(!table[4].inv.end);
	RX_REQUIRE(!table[5].inv.end);
	RX_REQUIRE(!table[6].inv.end);

	RX_REQUIRE(!table[0].inv.finish);
	RX_REQUIRE(!table[1].inv.finish);
	RX_REQUIRE(!table[2].inv.finish);
	RX_REQUIRE(!table[3].inv.finish);
	RX_REQUIRE(!table[4].inv.finish);
	RX_REQUIRE(!table[5].inv.finish);
	RX_REQUIRE(!table[6].inv.finish);
}

RX_TEST_CASE(stack, finish)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[0], my_update_false)       },
	};

	struct action *actions[10];
	struct action_stack st = {0};

	action_stack_init(&st, actions, 10);
	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_finish(&st);

	RX_REQUIRE(!table[0].inv.handle);
	RX_REQUIRE(!table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(table[0].inv.end);
	RX_REQUIRE(table[0].inv.finish);

	RX_REQUIRE(!table[0].inv.handle);
	RX_REQUIRE(!table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(table[0].inv.end);
	RX_REQUIRE(table[0].inv.finish);
}

static const struct rx_test_case tests[] = {
	TEST(basics, handle),
	TEST(basics, update),
	TEST(basics, draw),
	TEST(basics, end),
	TEST(basics, finish),
	TEST(stack, add),
	TEST(stack, handle),
	TEST(stack, update),
	TEST(stack, draw),
	TEST(stack, finish)
};

int
main(int argc, char **argv)
{
	return TEST_RUN_ALL(tests, argc, argv);
}

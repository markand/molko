/*
 * test-script.c -- test script action
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

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <core/action.h>
#include <core/event.h>
#include <core/script.h>

struct invokes {
	int handle;
	int update;
	int draw;
	int end;
	int finish;
};

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

	((struct invokes *)act->data)->handle++;
}

static int
my_update_false(struct action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update++;

	return 0;
}

static int
my_update_true(struct action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update++;

	return 1;
}

static void
my_draw(struct action *act)
{
	((struct invokes *)act->data)->draw++;
}

static void
my_end(struct action *act)
{
	((struct invokes *)act->data)->end++;
}

static void
my_finish(struct action *act)
{
	((struct invokes *)act->data)->finish++;
}

GREATEST_TEST
basics_init(void)
{
	struct script sc;

	script_init(&sc);

	GREATEST_ASSERT_EQ(sc.actionsz, 0U);
	GREATEST_ASSERT_EQ(sc.cur, 0U);
	
	GREATEST_PASS();
}

GREATEST_TEST
basics_append(void)
{
	struct action act[3] = {0};
	struct script sc = {0};

	GREATEST_ASSERT(script_append(&sc, &act[0]) == 0);
	GREATEST_ASSERT_EQ(sc.cur, 0U);
	GREATEST_ASSERT_EQ(sc.actionsz, 1U);
	GREATEST_ASSERT_EQ(sc.actions[0], &act[0]);

	GREATEST_ASSERT(script_append(&sc, &act[1]) == 0);
	GREATEST_ASSERT_EQ(sc.cur, 0U);
	GREATEST_ASSERT_EQ(sc.actionsz, 2U);
	GREATEST_ASSERT_EQ(sc.actions[0], &act[0]);
	GREATEST_ASSERT_EQ(sc.actions[1], &act[1]);

	GREATEST_ASSERT(script_append(&sc, &act[2]) == 0);
	GREATEST_ASSERT_EQ(sc.cur, 0U);
	GREATEST_ASSERT_EQ(sc.actionsz, 3U);
	GREATEST_ASSERT_EQ(sc.actions[0], &act[0]);
	GREATEST_ASSERT_EQ(sc.actions[1], &act[1]);
	GREATEST_ASSERT_EQ(sc.actions[2], &act[2]);

	script_finish(&sc);

	GREATEST_PASS();
}

GREATEST_TEST
basics_handle(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};
	
	struct script sc = {0};

	GREATEST_ASSERT(script_append(&sc, &table[0].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[1].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* [0] */
	script_handle(&sc, &(union event){0});
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 0);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* [0] -> [1] */
	GREATEST_ASSERT(!script_update(&sc, 0));
	script_handle(&sc, &(union event){0});

	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* [2] */
	GREATEST_ASSERT(!script_update(&sc, 0));
	script_handle(&sc, &(union event){0});

	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_update(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};
	
	struct script sc = {0};

	GREATEST_ASSERT(script_append(&sc, &table[0].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[1].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* 0 -> 1 */
	GREATEST_ASSERT(!script_update(&sc, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* 1 -> 2 */
	GREATEST_ASSERT(!script_update(&sc, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* 2 stays, it never ends. */
	GREATEST_ASSERT(!script_update(&sc, 0));
	GREATEST_ASSERT(!script_update(&sc, 0));
	GREATEST_ASSERT(!script_update(&sc, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 3);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* Now, change its update function to complete the script. */
	table[2].act.update = my_update_true;
	GREATEST_ASSERT(script_update(&sc, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 4);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 1);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_draw(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};
	
	struct script sc = {0};

	GREATEST_ASSERT(script_append(&sc, &table[0].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[1].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* [0] */
	script_draw(&sc);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 0);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 0);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* [0] -> [1] */
	GREATEST_ASSERT(!script_update(&sc, 0));
	script_draw(&sc);

	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* [2] */
	GREATEST_ASSERT(!script_update(&sc, 0));
	script_draw(&sc);

	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	GREATEST_PASS();
}

GREATEST_TEST
basics_finish(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};
	
	struct script sc = {0};

	GREATEST_ASSERT(script_append(&sc, &table[0].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[1].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* Update once so that the current action goes to 1. */
	GREATEST_ASSERT(!script_update(&sc, 0));

	script_finish(&sc);

	GREATEST_ASSERT_EQ(table[0].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 1);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 1);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 1);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(basics_init);
	GREATEST_RUN_TEST(basics_append);
	GREATEST_RUN_TEST(basics_handle);
	GREATEST_RUN_TEST(basics_update);
	GREATEST_RUN_TEST(basics_draw);
	GREATEST_RUN_TEST(basics_finish);
}

GREATEST_TEST
action_simple(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};
	
	struct script sc = {0};
	struct action act;

	GREATEST_ASSERT(script_append(&sc, &table[0].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[1].act) == 0);
	GREATEST_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* Now convert this script into an action itself. */
	script_action(&sc, &act);

	/* Draw and input before updating. */
	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* [0] -> [1] */
	GREATEST_ASSERT(!action_update(&act, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[1].inv.update, 0);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[1].inv.end, 0);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* [1] -> [2] */
	GREATEST_ASSERT(!action_update(&act, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 0);
	GREATEST_ASSERT_EQ(table[2].inv.update, 0);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 0);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* 2 stays, it never ends. */
	GREATEST_ASSERT(!action_update(&act, 0));
	GREATEST_ASSERT(!action_update(&act, 0));
	GREATEST_ASSERT(!action_update(&act, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[2].inv.update, 3);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[2].inv.end, 0);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	table[2].act.update = my_update_true;
	GREATEST_ASSERT(action_update(&act, 0));
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 0);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[2].inv.update, 4);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[2].inv.end, 1);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 0);

	/* Also dispose resources. */
	action_finish(&act);
	GREATEST_ASSERT_EQ(table[0].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[0].inv.update, 1);
	GREATEST_ASSERT_EQ(table[0].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[0].inv.end, 1);
	GREATEST_ASSERT_EQ(table[0].inv.finish, 1);
	GREATEST_ASSERT_EQ(table[1].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[1].inv.update, 1);
	GREATEST_ASSERT_EQ(table[1].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[1].inv.end, 1);
	GREATEST_ASSERT_EQ(table[1].inv.finish, 1);
	GREATEST_ASSERT_EQ(table[2].inv.handle, 1);
	GREATEST_ASSERT_EQ(table[2].inv.update, 4);
	GREATEST_ASSERT_EQ(table[2].inv.draw, 1);
	GREATEST_ASSERT_EQ(table[2].inv.end, 1);
	GREATEST_ASSERT_EQ(table[2].inv.finish, 1);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_action)
{
	GREATEST_RUN_TEST(action_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_RUN_SUITE(suite_action);
	GREATEST_MAIN_END();
}

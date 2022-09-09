/*
 * test-script.c -- test script action
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
#include <core/event.h>
#include <core/script.h>

#include <dt.h>

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

static void
test_basics_init(void)
{
	struct script sc;

	script_init(&sc);

	DT_EQ_UINT(sc.actionsz, 0U);
	DT_EQ_UINT(sc.cur, 0U);
}

static void
test_basics_append(void)
{
	struct action act[3] = {0};
	struct script sc = {0};

	DT_ASSERT(script_append(&sc, &act[0]) == 0);
	DT_EQ_UINT(sc.cur, 0U);
	DT_EQ_UINT(sc.actionsz, 1U);
	DT_EQ_PTR(sc.actions[0], &act[0]);

	DT_ASSERT(script_append(&sc, &act[1]) == 0);
	DT_EQ_UINT(sc.cur, 0U);
	DT_EQ_UINT(sc.actionsz, 2U);
	DT_EQ_PTR(sc.actions[0], &act[0]);
	DT_EQ_PTR(sc.actions[1], &act[1]);

	DT_ASSERT(script_append(&sc, &act[2]) == 0);
	DT_EQ_UINT(sc.cur, 0U);
	DT_EQ_UINT(sc.actionsz, 3U);
	DT_EQ_PTR(sc.actions[0], &act[0]);
	DT_EQ_PTR(sc.actions[1], &act[1]);
	DT_EQ_PTR(sc.actions[2], &act[2]);

	script_finish(&sc);
}

static void
test_basics_handle(void)
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

	DT_ASSERT(script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* [0] */
	script_handle(&sc, &(union event){0});
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 0);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 0);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* [0] -> [1] */
	DT_ASSERT(!script_update(&sc, 0));
	script_handle(&sc, &(union event){0});

	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* [2] */
	DT_ASSERT(!script_update(&sc, 0));
	script_handle(&sc, &(union event){0});

	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 1);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);
}

static void
test_basics_update(void)
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

	DT_ASSERT(script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* 0 -> 1 */
	DT_ASSERT(!script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* 1 -> 2 */
	DT_ASSERT(!script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* 2 stays, it never ends. */
	DT_ASSERT(!script_update(&sc, 0));
	DT_ASSERT(!script_update(&sc, 0));
	DT_ASSERT(!script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 3);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* Now, change its update function to complete the script. */
	table[2].act.update = my_update_true;
	DT_ASSERT(script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 4);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 1);
	DT_EQ_INT(table[2].inv.finish, 0);
}

static void
test_basics_draw(void)
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

	DT_ASSERT(script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* [0] */
	script_draw(&sc);
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 0);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 0);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* [0] -> [1] */
	DT_ASSERT(!script_update(&sc, 0));
	script_draw(&sc);

	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* [2] */
	DT_ASSERT(!script_update(&sc, 0));
	script_draw(&sc);

	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 1);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);
}

static void
test_basics_finish(void)
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

	DT_ASSERT(script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* Update once so that the current action goes to 1. */
	DT_ASSERT(!script_update(&sc, 0));

	script_finish(&sc);

	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 1);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 1);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 1);
}

static void
test_action_simple(void)
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

	DT_ASSERT(script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(script_append(&sc, &table[2].act) == 0);

	/* Now convert this script into an action itself. */
	script_action(&sc, &act);

	/* Draw and input before updating. */
	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* [0] -> [1] */
	DT_ASSERT(!action_update(&act, 0));
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* [1] -> [2] */
	DT_ASSERT(!action_update(&act, 0));
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	action_handle(&act, &(union event){0});
	action_draw(&act);

	/* 2 stays, it never ends. */
	DT_ASSERT(!action_update(&act, 0));
	DT_ASSERT(!action_update(&act, 0));
	DT_ASSERT(!action_update(&act, 0));
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 1);
	DT_EQ_INT(table[2].inv.update, 3);
	DT_EQ_INT(table[2].inv.draw, 1);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	table[2].act.update = my_update_true;
	DT_ASSERT(action_update(&act, 0));
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.handle, 1);
	DT_EQ_INT(table[2].inv.update, 4);
	DT_EQ_INT(table[2].inv.draw, 1);
	DT_EQ_INT(table[2].inv.end, 1);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* Also dispose resources. */
	action_finish(&act);
	DT_EQ_INT(table[0].inv.handle, 1);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 1);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 1);
	DT_EQ_INT(table[1].inv.handle, 1);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 1);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 1);
	DT_EQ_INT(table[2].inv.handle, 1);
	DT_EQ_INT(table[2].inv.update, 4);
	DT_EQ_INT(table[2].inv.draw, 1);
	DT_EQ_INT(table[2].inv.end, 1);
	DT_EQ_INT(table[2].inv.finish, 1);
}

int
main(void)
{
	DT_RUN(test_basics_init);
	DT_RUN(test_basics_append);
	DT_RUN(test_basics_handle);
	DT_RUN(test_basics_update);
	DT_RUN(test_basics_draw);
	DT_RUN(test_basics_finish);
	DT_RUN(test_action_simple);
	DT_SUMMARY();
}

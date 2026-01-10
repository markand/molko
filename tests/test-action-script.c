/*
 * test-script.c -- test script action
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <mlk/core/action-script.h>
#include <mlk/core/action.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/util.h>

#include <dt.h>

struct invokes {
	int start;
	int handle;
	int update;
	int draw;
	int end;
	int finish;
};

#define INIT(dat, up) {         \
	.data = (dat),          \
	.start = my_start,      \
	.handle = my_handle,    \
	.update = (up),         \
	.draw = my_draw,        \
	.end = my_end,          \
	.finish = my_finish     \
}

static void
my_start(struct mlk_action *act)
{
	((struct invokes *)act->data)->start++;
}

static void
my_handle(struct mlk_action *act, const union mlk_event *ev)
{
	(void)ev;

	((struct invokes *)act->data)->handle++;
}

static int
my_update_false(struct mlk_action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update++;

	return 0;
}

static int
my_update_true(struct mlk_action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update++;

	return 1;
}

static void
my_draw(struct mlk_action *act)
{
	((struct invokes *)act->data)->draw++;
}

static void
my_end(struct mlk_action *act)
{
	((struct invokes *)act->data)->end++;
}

static void
my_finish(struct mlk_action *act)
{
	((struct invokes *)act->data)->finish++;
}

static void
test_basics_init(void)
{
	struct mlk_action *actions[10];
	struct mlk_action_script sc = {
		.actions = actions,
		.actionsz = MLK_UTIL_SIZE(actions)
	};

	mlk_action_script_init(&sc);

	DT_EQ_SIZE(sc.actionsz, 10U);
	DT_EQ_SIZE(sc.length, 0U);
	DT_EQ_SIZE(sc.current, 0U);

	mlk_action_script_finish(&sc);

	DT_EQ_PTR(sc.actions, actions);
	DT_EQ_SIZE(sc.actionsz, 10U);
	DT_EQ_SIZE(sc.length, 0U);
	DT_EQ_SIZE(sc.current, 0U);
}

static void
test_basics_append(void)
{
	struct mlk_action actions[4] = {0};
	struct mlk_action *array[3] = {0};
	struct mlk_action_script sc = {
		.actions = array,
		.actionsz = MLK_UTIL_SIZE(array)
	};

	mlk_action_script_init(&sc);

	DT_ASSERT(mlk_action_script_append(&sc, &actions[0]) == 0);
	DT_EQ_SIZE(sc.actionsz, 3U);
	DT_EQ_SIZE(sc.length, 1U);
	DT_EQ_SIZE(sc.current, 0U);
	DT_EQ_PTR(sc.actions[0], &actions[0]);

	DT_ASSERT(mlk_action_script_append(&sc, &actions[1]) == 0);
	DT_EQ_SIZE(sc.actionsz, 3U);
	DT_EQ_SIZE(sc.length, 2U);
	DT_EQ_SIZE(sc.current, 0U);
	DT_EQ_PTR(sc.actions[0], &actions[0]);
	DT_EQ_PTR(sc.actions[1], &actions[1]);

	DT_ASSERT(mlk_action_script_append(&sc, &actions[2]) == 0);
	DT_EQ_SIZE(sc.actionsz, 3U);
	DT_EQ_SIZE(sc.length, 3U);
	DT_EQ_SIZE(sc.current, 0U);
	DT_EQ_PTR(sc.actions[0], &actions[0]);
	DT_EQ_PTR(sc.actions[1], &actions[1]);
	DT_EQ_PTR(sc.actions[2], &actions[2]);

	/* This can not fit. */
	DT_ASSERT(mlk_action_script_append(&sc, &actions[3]) == -1);

	mlk_action_script_finish(&sc);

	DT_EQ_PTR(sc.actions, array);
	DT_EQ_SIZE(sc.actionsz, 3U);
	DT_EQ_SIZE(sc.length, 0U);
	DT_EQ_SIZE(sc.current, 0U);
}

static void
test_basics_handle(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};

	struct mlk_action *array[3] = {0};
	struct mlk_action_script sc = {
		.actions = array,
		.actionsz = MLK_UTIL_SIZE(array)
	};

	mlk_action_script_init(&sc);

	DT_ASSERT(mlk_action_script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[2].act) == 0);

	/* [0] */
	mlk_action_script_handle(&sc, &(const union mlk_event){0});
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
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	mlk_action_script_handle(&sc, &(union mlk_event){0});

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
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	mlk_action_script_handle(&sc, &(union mlk_event){0});

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

	mlk_action_script_finish(&sc);
}

static void
test_basics_update(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};

	struct mlk_action *array[3];
	struct mlk_action_script sc = {
		.actions = array,
		.actionsz = MLK_UTIL_SIZE(array)
	};

	mlk_action_script_init(&sc);

	DT_ASSERT(mlk_action_script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[2].act) == 0);

	mlk_action_script_start(&sc);

	/* 0 -> 1 */
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.start, 1);
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.start, 1);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 0);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 0);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.start, 0);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* 1 -> 2 */
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.start, 1);
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.start, 1);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.start, 1);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 0);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* 2 stays, it never ends. */
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.start, 1);
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.start, 1);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.start, 1);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 3);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 0);
	DT_EQ_INT(table[2].inv.finish, 0);

	/* Now, change its update function to complete the script. */
	table[2].act.update = my_update_true;
	DT_ASSERT(mlk_action_script_update(&sc, 0));
	DT_EQ_INT(table[0].inv.start, 1);
	DT_EQ_INT(table[0].inv.handle, 0);
	DT_EQ_INT(table[0].inv.update, 1);
	DT_EQ_INT(table[0].inv.draw, 0);
	DT_EQ_INT(table[0].inv.end, 1);
	DT_EQ_INT(table[0].inv.finish, 0);
	DT_EQ_INT(table[1].inv.start, 1);
	DT_EQ_INT(table[1].inv.handle, 0);
	DT_EQ_INT(table[1].inv.update, 1);
	DT_EQ_INT(table[1].inv.draw, 0);
	DT_EQ_INT(table[1].inv.end, 1);
	DT_EQ_INT(table[1].inv.finish, 0);
	DT_EQ_INT(table[2].inv.start, 1);
	DT_EQ_INT(table[2].inv.handle, 0);
	DT_EQ_INT(table[2].inv.update, 4);
	DT_EQ_INT(table[2].inv.draw, 0);
	DT_EQ_INT(table[2].inv.end, 1);
	DT_EQ_INT(table[2].inv.finish, 0);

	mlk_action_script_finish(&sc);
}

static void
test_basics_draw(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};

	struct mlk_action *array[3];
	struct mlk_action_script sc = {
		.actions = array,
		.actionsz = MLK_UTIL_SIZE(array)
	};

	mlk_action_script_init(&sc);

	DT_ASSERT(mlk_action_script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[2].act) == 0);

	/* [0] */
	mlk_action_script_draw(&sc);
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
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	mlk_action_script_draw(&sc);

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
	DT_ASSERT(!mlk_action_script_update(&sc, 0));
	mlk_action_script_draw(&sc);

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
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0].inv, my_update_true)    },
		{ .act = INIT(&table[1].inv, my_update_true)    },
		{ .act = INIT(&table[2].inv, my_update_false)   }
	};

	struct mlk_action *array[3];
	struct mlk_action_script sc = {
		.actions = array,
		.actionsz = MLK_UTIL_SIZE(array)
	};

	mlk_action_script_init(&sc);

	DT_ASSERT(mlk_action_script_append(&sc, &table[0].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[1].act) == 0);
	DT_ASSERT(mlk_action_script_append(&sc, &table[2].act) == 0);

	/* Update once so that the current action goes to 1. */
	DT_ASSERT(!mlk_action_script_update(&sc, 0));

	mlk_action_script_finish(&sc);

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

int
main(void)
{
	DT_RUN(test_basics_init);
	DT_RUN(test_basics_append);
	DT_RUN(test_basics_handle);
	DT_RUN(test_basics_update);
	DT_RUN(test_basics_draw);
	DT_RUN(test_basics_finish);
	DT_SUMMARY();
}

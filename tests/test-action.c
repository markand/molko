/*
 * test-action.c -- test actions
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

#include <mlk/core/action-stack.h>
#include <mlk/core/action.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>

#include <dt.h>

struct invokes {
	int handle;
	int update;
	int draw;
	int end;
	int finish;
};

static union mlk_event dummy;

#define INIT(dat, up) {         \
        .data = (dat),          \
        .handle = my_handle,    \
        .update = (up),         \
        .draw = my_draw,        \
        .end = my_end,          \
        .finish = my_finish     \
}

static void
my_handle(struct mlk_action *act, const union mlk_event *ev)
{
	(void)ev;

	((struct invokes *)act->data)->handle = 1;
}

static int
my_update_false(struct mlk_action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update = 1;

	return 0;
}

static int
my_update_true(struct mlk_action *act, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)act->data)->update = 1;

	return 1;
}

static void
my_draw(struct mlk_action *act)
{
	((struct invokes *)act->data)->draw = 1;
}

static void
my_end(struct mlk_action *act)
{
	((struct invokes *)act->data)->end = 1;
}

static void
my_finish(struct mlk_action *act)
{
	((struct invokes *)act->data)->finish = 1;
}

static void
test_basics_handle(void)
{
	struct invokes inv = {0};
	struct mlk_action act = INIT(&inv, my_update_true);

	mlk_action_handle(&act, &dummy);

	DT_ASSERT(inv.handle);
	DT_ASSERT(!inv.update);
	DT_ASSERT(!inv.draw);
	DT_ASSERT(!inv.end);
	DT_ASSERT(!inv.finish);
}

static void
test_basics_update(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[1], my_update_false)       }
	};

	/* True version. */
	DT_ASSERT(mlk_action_update(&table[0].act, 0));
	DT_ASSERT(!table[0].inv.handle);
	DT_ASSERT(table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(!table[0].inv.end);
	DT_ASSERT(!table[0].inv.finish);

	/* False version. */
	DT_ASSERT(!mlk_action_update(&table[1].act, 0));
	DT_ASSERT(!table[1].inv.handle);
	DT_ASSERT(table[1].inv.update);
	DT_ASSERT(!table[1].inv.draw);
	DT_ASSERT(!table[1].inv.end);
	DT_ASSERT(!table[1].inv.finish);
}

static void
test_basics_draw(void)
{
	struct invokes inv = {0};
	struct mlk_action act = INIT(&inv, my_update_true);

	mlk_action_draw(&act);

	DT_ASSERT(!inv.handle);
	DT_ASSERT(!inv.update);
	DT_ASSERT(inv.draw);
	DT_ASSERT(!inv.end);
	DT_ASSERT(!inv.finish);
}

static void
test_basics_end(void)
{
	struct invokes inv = {0};
	struct mlk_action act = INIT(&inv, my_update_true);

	mlk_action_end(&act);

	DT_ASSERT(!inv.handle);
	DT_ASSERT(!inv.update);
	DT_ASSERT(!inv.draw);
	DT_ASSERT(inv.end);
	DT_ASSERT(!inv.finish);
}

static void
test_basics_finish(void)
{
	struct invokes inv = {0};
	struct mlk_action act = INIT(&inv, my_update_true);

	mlk_action_finish(&act);

	DT_ASSERT(!inv.handle);
	DT_ASSERT(!inv.update);
	DT_ASSERT(!inv.draw);
	DT_ASSERT(!inv.end);
	DT_ASSERT(inv.finish);
}

static void
test_stack_add(void)
{
	struct mlk_action *actions[10];
	struct mlk_action_stack st = {0};
	struct mlk_action act = {0};

	mlk_action_stack_init(&st, actions, 10);

	DT_EQ_INT(mlk_action_stack_add(&st, &act), 0);

	/* Now fill up. */
	for (int i = 0; i < 9; ++i)
		DT_EQ_INT(mlk_action_stack_add(&st, &act), 0);

	/* This one should not fit in. */
	DT_EQ_INT(mlk_action_stack_add(&st, &act), MLK_ERR_NO_MEM);
}

static void
test_stack_handle(void)
{
	struct {
		int called;
		struct mlk_action act;
	} table[] = {
		{ 0, { .data = &table[0].called, .handle = my_handle } },
		{ 0, { .data = &table[1].called, .handle = my_handle } },
		{ 0, { .data = &table[2].called, .handle = my_handle } },
	};

	struct mlk_action *actions[10];
	struct mlk_action_stack st = {0};

	mlk_action_stack_init(&st, actions, 10);
	mlk_action_stack_add(&st, &table[0].act);
	mlk_action_stack_add(&st, &table[1].act);
	mlk_action_stack_add(&st, &table[2].act);
	mlk_action_stack_handle(&st, &dummy);

	DT_ASSERT(table[0].called);
	DT_ASSERT(table[1].called);
	DT_ASSERT(table[2].called);
}

static void
test_stack_update(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_false)       },
		{ .act = INIT(&table[1], my_update_true)        },
		{ .act = INIT(&table[2], my_update_false)       },
		{ .act = INIT(&table[3], my_update_false)       },
		{ .act = INIT(&table[4], my_update_true)        },
		{ .act = INIT(&table[5], my_update_true)        },
		{ .act = INIT(&table[6], my_update_false)	},
	};

	struct mlk_action *actions[10];
	struct mlk_action_stack st = {0};

	mlk_action_stack_init(&st, actions, 10);
	mlk_action_stack_add(&st, &table[0].act);
	mlk_action_stack_add(&st, &table[1].act);
	mlk_action_stack_add(&st, &table[2].act);
	mlk_action_stack_add(&st, &table[3].act);
	mlk_action_stack_add(&st, &table[4].act);
	mlk_action_stack_add(&st, &table[5].act);
	mlk_action_stack_add(&st, &table[6].act);

	DT_ASSERT(!mlk_action_stack_update(&st, 0));

	DT_ASSERT(!table[0].inv.handle);
	DT_ASSERT(!table[1].inv.handle);
	DT_ASSERT(!table[2].inv.handle);
	DT_ASSERT(!table[3].inv.handle);
	DT_ASSERT(!table[4].inv.handle);
	DT_ASSERT(!table[5].inv.handle);
	DT_ASSERT(!table[6].inv.handle);

	DT_ASSERT(table[0].inv.update);
	DT_ASSERT(table[1].inv.update);
	DT_ASSERT(table[2].inv.update);
	DT_ASSERT(table[3].inv.update);
	DT_ASSERT(table[4].inv.update);
	DT_ASSERT(table[5].inv.update);
	DT_ASSERT(table[6].inv.update);

	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(!table[1].inv.draw);
	DT_ASSERT(!table[2].inv.draw);
	DT_ASSERT(!table[3].inv.draw);
	DT_ASSERT(!table[4].inv.draw);
	DT_ASSERT(!table[5].inv.draw);
	DT_ASSERT(!table[6].inv.draw);

	DT_ASSERT(!table[0].inv.end);
	DT_ASSERT(table[1].inv.end);
	DT_ASSERT(!table[2].inv.end);
	DT_ASSERT(!table[3].inv.end);
	DT_ASSERT(table[4].inv.end);
	DT_ASSERT(table[5].inv.end);
	DT_ASSERT(!table[6].inv.end);

	DT_ASSERT(!table[0].inv.finish);
	DT_ASSERT(table[1].inv.finish);
	DT_ASSERT(!table[2].inv.finish);
	DT_ASSERT(!table[3].inv.finish);
	DT_ASSERT(table[4].inv.finish);
	DT_ASSERT(table[5].inv.finish);
	DT_ASSERT(!table[6].inv.finish);

	/* The following must still be there. */
	DT_EQ_PTR(st.actions[0], &table[0].act);
	DT_EQ_PTR(st.actions[2], &table[2].act);
	DT_EQ_PTR(st.actions[3], &table[3].act);
	DT_EQ_PTR(st.actions[6], &table[6].act);

	/* The following must have been NULL-ed. */
	DT_EQ_PTR(st.actions[1], NULL);
	DT_EQ_PTR(st.actions[4], NULL);
	DT_EQ_PTR(st.actions[5], NULL);

	/*
	 * Now make all actions to return 1 and check if it cleans the stack.
	 */
	table[0].act.update =
	    table[2].act.update =
	    table[3].act.update =
	    table[6].act.update = my_update_true;

	DT_ASSERT(mlk_action_stack_update(&st, 0));
	DT_EQ_PTR(st.actions[0], NULL);
	DT_EQ_PTR(st.actions[1], NULL);
	DT_EQ_PTR(st.actions[2], NULL);
	DT_EQ_PTR(st.actions[3], NULL);
	DT_EQ_PTR(st.actions[4], NULL);
	DT_EQ_PTR(st.actions[5], NULL);
	DT_EQ_PTR(st.actions[6], NULL);
}

static void
test_stack_draw(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_false)       },
		{ .act = INIT(&table[1], my_update_true)        },
		{ .act = INIT(&table[2], my_update_false)       },
		{ .act = INIT(&table[3], my_update_false)       },
		{ .act = INIT(&table[4], my_update_true)        },
		{ .act = INIT(&table[5], my_update_true)        },
		{ .act = INIT(&table[6], my_update_false)	},
	};

	struct mlk_action *actions[10];
	struct mlk_action_stack st = {0};

	mlk_action_stack_init(&st, actions, 10);
	mlk_action_stack_add(&st, &table[0].act);
	mlk_action_stack_add(&st, &table[1].act);
	mlk_action_stack_add(&st, &table[2].act);
	mlk_action_stack_add(&st, &table[3].act);
	mlk_action_stack_add(&st, &table[4].act);
	mlk_action_stack_add(&st, &table[5].act);
	mlk_action_stack_add(&st, &table[6].act);
	mlk_action_stack_draw(&st);

	DT_ASSERT(!table[0].inv.handle);
	DT_ASSERT(!table[1].inv.handle);
	DT_ASSERT(!table[2].inv.handle);
	DT_ASSERT(!table[3].inv.handle);
	DT_ASSERT(!table[4].inv.handle);
	DT_ASSERT(!table[5].inv.handle);
	DT_ASSERT(!table[6].inv.handle);

	DT_ASSERT(!table[0].inv.update);
	DT_ASSERT(!table[1].inv.update);
	DT_ASSERT(!table[2].inv.update);
	DT_ASSERT(!table[3].inv.update);
	DT_ASSERT(!table[4].inv.update);
	DT_ASSERT(!table[5].inv.update);
	DT_ASSERT(!table[6].inv.update);

	DT_ASSERT(table[0].inv.draw);
	DT_ASSERT(table[1].inv.draw);
	DT_ASSERT(table[2].inv.draw);
	DT_ASSERT(table[3].inv.draw);
	DT_ASSERT(table[4].inv.draw);
	DT_ASSERT(table[5].inv.draw);
	DT_ASSERT(table[6].inv.draw);

	DT_ASSERT(!table[0].inv.end);
	DT_ASSERT(!table[1].inv.end);
	DT_ASSERT(!table[2].inv.end);
	DT_ASSERT(!table[3].inv.end);
	DT_ASSERT(!table[4].inv.end);
	DT_ASSERT(!table[5].inv.end);
	DT_ASSERT(!table[6].inv.end);

	DT_ASSERT(!table[0].inv.finish);
	DT_ASSERT(!table[1].inv.finish);
	DT_ASSERT(!table[2].inv.finish);
	DT_ASSERT(!table[3].inv.finish);
	DT_ASSERT(!table[4].inv.finish);
	DT_ASSERT(!table[5].inv.finish);
	DT_ASSERT(!table[6].inv.finish);
}

static void
test_stack_finish(void)
{
	struct {
		struct invokes inv;
		struct mlk_action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[0], my_update_false)       },
	};

	struct mlk_action *actions[10];
	struct mlk_action_stack st = {0};

	mlk_action_stack_init(&st, actions, 10);
	mlk_action_stack_add(&st, &table[0].act);
	mlk_action_stack_add(&st, &table[1].act);
	mlk_action_stack_finish(&st);

	DT_ASSERT(!table[0].inv.handle);
	DT_ASSERT(!table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(table[0].inv.end);
	DT_ASSERT(table[0].inv.finish);

	DT_ASSERT(!table[0].inv.handle);
	DT_ASSERT(!table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(table[0].inv.end);
	DT_ASSERT(table[0].inv.finish);
}

int
main(void)
{
	DT_RUN(test_basics_handle);
	DT_RUN(test_basics_update);
	DT_RUN(test_basics_draw);
	DT_RUN(test_basics_end);
	DT_RUN(test_basics_finish);
	DT_RUN(test_stack_add);
	DT_RUN(test_stack_handle);
	DT_RUN(test_stack_update);
	DT_RUN(test_stack_draw);
	DT_RUN(test_stack_finish);
	DT_SUMMARY();
}

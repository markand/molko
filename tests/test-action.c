/*
 * test-action.c -- test actions
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

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <core/action.h>
#include <core/event.h>

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

GREATEST_TEST
basics_handle(void)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_handle(&act, &dummy);

	GREATEST_ASSERT(inv.handle);
	GREATEST_ASSERT(!inv.update);
	GREATEST_ASSERT(!inv.draw);
	GREATEST_ASSERT(!inv.end);
	GREATEST_ASSERT(!inv.finish);
	GREATEST_PASS();
}

GREATEST_TEST
basics_update(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[1], my_update_false)       }
	};

	/* True version. */
	GREATEST_ASSERT(action_update(&table[0].act, 0));
	GREATEST_ASSERT(!table[0].inv.handle);
	GREATEST_ASSERT(table[0].inv.update);
	GREATEST_ASSERT(!table[0].inv.draw);
	GREATEST_ASSERT(!table[0].inv.end);
	GREATEST_ASSERT(!table[0].inv.finish);

	/* False version. */
	GREATEST_ASSERT(!action_update(&table[1].act, 0));
	GREATEST_ASSERT(!table[1].inv.handle);
	GREATEST_ASSERT(table[1].inv.update);
	GREATEST_ASSERT(!table[1].inv.draw);
	GREATEST_ASSERT(!table[1].inv.end);
	GREATEST_ASSERT(!table[1].inv.finish);
	
	GREATEST_PASS();
}

GREATEST_TEST
basics_draw(void)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_draw(&act);

	GREATEST_ASSERT(!inv.handle);
	GREATEST_ASSERT(!inv.update);
	GREATEST_ASSERT(inv.draw);
	GREATEST_ASSERT(!inv.end);
	GREATEST_ASSERT(!inv.finish);
	GREATEST_PASS();
}

GREATEST_TEST
basics_end(void)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_end(&act);

	GREATEST_ASSERT(!inv.handle);
	GREATEST_ASSERT(!inv.update);
	GREATEST_ASSERT(!inv.draw);
	GREATEST_ASSERT(inv.end);
	GREATEST_ASSERT(!inv.finish);
	GREATEST_PASS();
}

GREATEST_TEST
basics_finish(void)
{
	struct invokes inv = {0};
	struct action act = INIT(&inv, my_update_true);

	action_finish(&act);

	GREATEST_ASSERT(!inv.handle);
	GREATEST_ASSERT(!inv.update);
	GREATEST_ASSERT(!inv.draw);
	GREATEST_ASSERT(!inv.end);
	GREATEST_ASSERT(inv.finish);
	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(basics_handle);
	GREATEST_RUN_TEST(basics_update);
	GREATEST_RUN_TEST(basics_draw);
	GREATEST_RUN_TEST(basics_end);
	GREATEST_RUN_TEST(basics_finish);
}

GREATEST_TEST
stack_add(void)
{
	struct action_stack st = {0};
	struct action act = {0};

	GREATEST_ASSERT(action_stack_add(&st, &act) == 0);

	/* Now fill up. */
	for (int i = 0; i < ACTION_STACK_MAX - 1; ++i)
		GREATEST_ASSERT(action_stack_add(&st, &act) == 0);

	/* This one should not fit in. */
	GREATEST_ASSERT(action_stack_add(&st, &act) < 0);

	GREATEST_PASS();
}

GREATEST_TEST
stack_handle(void)
{
	struct {
		int called;
		struct action act;
	} table[] = {
		{ 0, { .data = &table[0].called, .handle = my_handle } },
		{ 0, { .data = &table[1].called, .handle = my_handle } },
		{ 0, { .data = &table[2].called, .handle = my_handle } },
	};

	struct action_stack st = {0};

	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_add(&st, &table[2].act);
	action_stack_handle(&st, &dummy);

	GREATEST_ASSERT(table[0].called);
	GREATEST_ASSERT(table[1].called);
	GREATEST_ASSERT(table[2].called);
	GREATEST_PASS();
}

GREATEST_TEST
stack_update(void)
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

	struct action_stack st = {0};

	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_add(&st, &table[2].act);
	action_stack_add(&st, &table[3].act);
	action_stack_add(&st, &table[4].act);
	action_stack_add(&st, &table[5].act);
	action_stack_add(&st, &table[6].act);

	GREATEST_ASSERT(!action_stack_update(&st, 0));

	GREATEST_ASSERT(!table[0].inv.handle);
	GREATEST_ASSERT(!table[1].inv.handle);
	GREATEST_ASSERT(!table[2].inv.handle);
	GREATEST_ASSERT(!table[3].inv.handle);
	GREATEST_ASSERT(!table[4].inv.handle);
	GREATEST_ASSERT(!table[5].inv.handle);
	GREATEST_ASSERT(!table[6].inv.handle);

	GREATEST_ASSERT(table[0].inv.update);
	GREATEST_ASSERT(table[1].inv.update);
	GREATEST_ASSERT(table[2].inv.update);
	GREATEST_ASSERT(table[3].inv.update);
	GREATEST_ASSERT(table[4].inv.update);
	GREATEST_ASSERT(table[5].inv.update);
	GREATEST_ASSERT(table[6].inv.update);

	GREATEST_ASSERT(!table[0].inv.draw);
	GREATEST_ASSERT(!table[1].inv.draw);
	GREATEST_ASSERT(!table[2].inv.draw);
	GREATEST_ASSERT(!table[3].inv.draw);
	GREATEST_ASSERT(!table[4].inv.draw);
	GREATEST_ASSERT(!table[5].inv.draw);
	GREATEST_ASSERT(!table[6].inv.draw);

	GREATEST_ASSERT(!table[0].inv.end);
	GREATEST_ASSERT(table[1].inv.end);
	GREATEST_ASSERT(!table[2].inv.end);
	GREATEST_ASSERT(!table[3].inv.end);
	GREATEST_ASSERT(table[4].inv.end);
	GREATEST_ASSERT(table[5].inv.end);
	GREATEST_ASSERT(!table[6].inv.end);

	GREATEST_ASSERT(!table[0].inv.finish);
	GREATEST_ASSERT(table[1].inv.finish);
	GREATEST_ASSERT(!table[2].inv.finish);
	GREATEST_ASSERT(!table[3].inv.finish);
	GREATEST_ASSERT(table[4].inv.finish);
	GREATEST_ASSERT(table[5].inv.finish);
	GREATEST_ASSERT(!table[6].inv.finish);

	/* The following must still be there. */
	GREATEST_ASSERT_EQ(st.actions[0], &table[0].act);
	GREATEST_ASSERT_EQ(st.actions[2], &table[2].act);
	GREATEST_ASSERT_EQ(st.actions[3], &table[3].act);
	GREATEST_ASSERT_EQ(st.actions[6], &table[6].act);

	/* The following must have been NULL-ed. */
	GREATEST_ASSERT_EQ(st.actions[1], NULL);
	GREATEST_ASSERT_EQ(st.actions[4], NULL);
	GREATEST_ASSERT_EQ(st.actions[5], NULL);
	
	/*
	 * Now make all actions to return 1 and check if it cleans the stack.
	 */
	table[0].act.update =
	    table[2].act.update =
	    table[3].act.update =
	    table[6].act.update = my_update_true;

	GREATEST_ASSERT(action_stack_update(&st, 0));
	GREATEST_ASSERT_EQ(st.actions[0], NULL);
	GREATEST_ASSERT_EQ(st.actions[1], NULL);
	GREATEST_ASSERT_EQ(st.actions[2], NULL);
	GREATEST_ASSERT_EQ(st.actions[3], NULL);
	GREATEST_ASSERT_EQ(st.actions[4], NULL);
	GREATEST_ASSERT_EQ(st.actions[5], NULL);
	GREATEST_ASSERT_EQ(st.actions[6], NULL);

	GREATEST_PASS();
}

GREATEST_TEST
stack_finish(void)
{
	struct {
		struct invokes inv;
		struct action act;
	} table[] = {
		{ .act = INIT(&table[0], my_update_true)        },
		{ .act = INIT(&table[0], my_update_false)       },
	};
	struct action_stack st = {0};

	action_stack_add(&st, &table[0].act);
	action_stack_add(&st, &table[1].act);
	action_stack_finish(&st);

	GREATEST_ASSERT(!table[0].inv.handle);
	GREATEST_ASSERT(!table[0].inv.update);
	GREATEST_ASSERT(!table[0].inv.draw);
	GREATEST_ASSERT(table[0].inv.end);
	GREATEST_ASSERT(table[0].inv.finish);

	GREATEST_ASSERT(!table[0].inv.handle);
	GREATEST_ASSERT(!table[0].inv.update);
	GREATEST_ASSERT(!table[0].inv.draw);
	GREATEST_ASSERT(table[0].inv.end);
	GREATEST_ASSERT(table[0].inv.finish);

	/* They should also be NULL'ed. */
	GREATEST_ASSERT_EQ(st.actions[0], NULL);
	GREATEST_ASSERT_EQ(st.actions[1], NULL);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_stack)
{
	GREATEST_RUN_TEST(stack_add);
	GREATEST_RUN_TEST(stack_handle);
	GREATEST_RUN_TEST(stack_update);
	GREATEST_RUN_TEST(stack_finish);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_RUN_SUITE(suite_stack);
	GREATEST_MAIN_END();

	return 0;
}

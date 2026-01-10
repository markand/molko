/*
 * test-action.c -- test actions
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

#include <mlk/core/util.h>

#include <mlk/core/drawable-stack.h>
#include <mlk/core/drawable.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>

#include <dt.h>

struct invokes {
	int update;
	int draw;
	int end;
	int finish;
};

#define INIT(dat, up) {         \
        .data = (dat),          \
        .update = (up),         \
        .draw = my_draw,        \
        .end = my_end,          \
        .finish = my_finish     \
}

static int
my_update_false(struct mlk_drawable *dw, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)dw->data)->update = 1;

	return 0;
}

static int
my_update_true(struct mlk_drawable *dw, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)dw->data)->update = 1;

	return 1;
}

static void
my_draw(struct mlk_drawable *dw)
{
	((struct invokes *)dw->data)->draw = 1;
}

static void
my_end(struct mlk_drawable *dw)
{
	((struct invokes *)dw->data)->end = 1;
}

static void
my_finish(struct mlk_drawable *dw)
{
	((struct invokes *)dw->data)->finish = 1;
}

static void
test_basics_update(void)
{
	struct {
		struct invokes inv;
		struct mlk_drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_true)        },
		{ .dw = INIT(&table[1], my_update_false)       }
	};

	/* True version. */
	DT_ASSERT(mlk_drawable_update(&table[0].dw, 0));
	DT_ASSERT(table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(!table[0].inv.end);
	DT_ASSERT(!table[0].inv.finish);

	/* False version. */
	DT_ASSERT(!mlk_drawable_update(&table[1].dw, 0));
	DT_ASSERT(table[1].inv.update);
	DT_ASSERT(!table[1].inv.draw);
	DT_ASSERT(!table[1].inv.end);
	DT_ASSERT(!table[1].inv.finish);
}

static void
test_basics_draw(void)
{
	struct invokes inv = {0};
	struct mlk_drawable dw = INIT(&inv, my_update_true);

	mlk_drawable_draw(&dw);

	DT_ASSERT(!inv.update);
	DT_ASSERT(inv.draw);
	DT_ASSERT(!inv.end);
	DT_ASSERT(!inv.finish);
}

static void
test_basics_end(void)
{
	struct invokes inv = {0};
	struct mlk_drawable dw = INIT(&inv, my_update_true);

	mlk_drawable_end(&dw);

	DT_ASSERT(!inv.update);
	DT_ASSERT(!inv.draw);
	DT_ASSERT(inv.end);
	DT_ASSERT(!inv.finish);
}

static void
test_basics_finish(void)
{
	struct invokes inv = {0};
	struct mlk_drawable dw = INIT(&inv, my_update_true);

	mlk_drawable_finish(&dw);

	DT_ASSERT(!inv.update);
	DT_ASSERT(!inv.draw);
	DT_ASSERT(!inv.end);
	DT_ASSERT(inv.finish);
}

static void
test_stack_add(void)
{
	struct mlk_drawable dw = {0};
	struct mlk_drawable *drawables[10];
	struct mlk_drawable_stack st = {
		.objects = drawables,
		.objectsz = MLK_UTIL_SIZE(drawables)
	};

	mlk_drawable_stack_init(&st);

	DT_EQ_INT(mlk_drawable_stack_add(&st, &dw), 0);

	/* Now fill up. */
	for (int i = 0; i < 9; ++i)
		DT_EQ_INT(mlk_drawable_stack_add(&st, &dw), 0);

	/* This one should not fit in. */
	DT_EQ_INT(mlk_drawable_stack_add(&st, &dw), -1);
}

static void
test_stack_update(void)
{
	struct {
		struct invokes inv;
		struct mlk_drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_false)       },
		{ .dw = INIT(&table[1], my_update_true)        },
		{ .dw = INIT(&table[2], my_update_false)       },
		{ .dw = INIT(&table[3], my_update_false)       },
		{ .dw = INIT(&table[4], my_update_true)        },
		{ .dw = INIT(&table[5], my_update_true)        },
		{ .dw = INIT(&table[6], my_update_false)	},
	};

	struct mlk_drawable *drawables[10];
	struct mlk_drawable_stack st = {
		.objects = drawables,
		.objectsz = MLK_UTIL_SIZE(drawables)
	};

	mlk_drawable_stack_init(&st);
	mlk_drawable_stack_add(&st, &table[0].dw);
	mlk_drawable_stack_add(&st, &table[1].dw);
	mlk_drawable_stack_add(&st, &table[2].dw);
	mlk_drawable_stack_add(&st, &table[3].dw);
	mlk_drawable_stack_add(&st, &table[4].dw);
	mlk_drawable_stack_add(&st, &table[5].dw);
	mlk_drawable_stack_add(&st, &table[6].dw);

	DT_ASSERT(!mlk_drawable_stack_update(&st, 0));

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
	DT_EQ_PTR(st.objects[0], &table[0].dw);
	DT_EQ_PTR(st.objects[2], &table[2].dw);
	DT_EQ_PTR(st.objects[3], &table[3].dw);
	DT_EQ_PTR(st.objects[6], &table[6].dw);

	/* The following must have been NULL-ed. */
	DT_EQ_PTR(st.objects[1], NULL);
	DT_EQ_PTR(st.objects[4], NULL);
	DT_EQ_PTR(st.objects[5], NULL);

	/*
	 * Now make all actions to return 1 and check if it cleans the stack.
	 */
	table[0].dw.update =
	    table[2].dw.update =
	    table[3].dw.update =
	    table[6].dw.update = my_update_true;

	DT_ASSERT(mlk_drawable_stack_update(&st, 0));
	DT_EQ_PTR(st.objects[0], NULL);
	DT_EQ_PTR(st.objects[1], NULL);
	DT_EQ_PTR(st.objects[2], NULL);
	DT_EQ_PTR(st.objects[3], NULL);
	DT_EQ_PTR(st.objects[4], NULL);
	DT_EQ_PTR(st.objects[5], NULL);
	DT_EQ_PTR(st.objects[6], NULL);
}

static void
test_stack_draw(void)
{
	struct {
		struct invokes inv;
		struct mlk_drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_false)       },
		{ .dw = INIT(&table[1], my_update_true)        },
		{ .dw = INIT(&table[2], my_update_false)       },
		{ .dw = INIT(&table[3], my_update_false)       },
		{ .dw = INIT(&table[4], my_update_true)        },
		{ .dw = INIT(&table[5], my_update_true)        },
		{ .dw = INIT(&table[6], my_update_false)	},
	};

	struct mlk_drawable *drawables[10];
	struct mlk_drawable_stack st = {
		.objects = drawables,
		.objectsz = MLK_UTIL_SIZE(drawables)
	};

	mlk_drawable_stack_init(&st);
	mlk_drawable_stack_add(&st, &table[0].dw);
	mlk_drawable_stack_add(&st, &table[1].dw);
	mlk_drawable_stack_add(&st, &table[2].dw);
	mlk_drawable_stack_add(&st, &table[3].dw);
	mlk_drawable_stack_add(&st, &table[4].dw);
	mlk_drawable_stack_add(&st, &table[5].dw);
	mlk_drawable_stack_add(&st, &table[6].dw);
	mlk_drawable_stack_draw(&st);

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
		struct mlk_drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_true)        },
		{ .dw = INIT(&table[0], my_update_false)       },
	};

	struct mlk_drawable *drawables[10];
	struct mlk_drawable_stack st = {
		.objects = drawables,
		.objectsz = MLK_UTIL_SIZE(drawables)
	};

	mlk_drawable_stack_init(&st);
	mlk_drawable_stack_add(&st, &table[0].dw);
	mlk_drawable_stack_add(&st, &table[1].dw);
	mlk_drawable_stack_finish(&st);

	DT_ASSERT(!table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(table[0].inv.end);
	DT_ASSERT(table[0].inv.finish);

	DT_ASSERT(!table[0].inv.update);
	DT_ASSERT(!table[0].inv.draw);
	DT_ASSERT(table[0].inv.end);
	DT_ASSERT(table[0].inv.finish);
}

int
main(void)
{
	DT_RUN(test_basics_update);
	DT_RUN(test_basics_draw);
	DT_RUN(test_basics_end);
	DT_RUN(test_basics_finish);
	DT_RUN(test_stack_add);
	DT_RUN(test_stack_update);
	DT_RUN(test_stack_draw);
	DT_RUN(test_stack_finish);
	DT_SUMMARY();
}

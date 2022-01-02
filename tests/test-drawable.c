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

#include <rexo.h>

#include <core/drawable.h>
#include <core/drawable-stack.h>
#include <core/event.h>

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
my_update_false(struct drawable *dw, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)dw->data)->update = 1;

	return 0;
}

static int
my_update_true(struct drawable *dw, unsigned int ticks)
{
	(void)ticks;

	((struct invokes *)dw->data)->update = 1;

	return 1;
}

static void
my_draw(struct drawable *dw)
{
	((struct invokes *)dw->data)->draw = 1;
}

static void
my_end(struct drawable *dw)
{
	((struct invokes *)dw->data)->end = 1;
}

static void
my_finish(struct drawable *dw)
{
	((struct invokes *)dw->data)->finish = 1;
}

RX_TEST_CASE(basics, update)
{
	struct {
		struct invokes inv;
		struct drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_true)        },
		{ .dw = INIT(&table[1], my_update_false)       }
	};

	/* True version. */
	RX_REQUIRE(drawable_update(&table[0].dw, 0));
	RX_REQUIRE(table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(!table[0].inv.end);
	RX_REQUIRE(!table[0].inv.finish);

	/* False version. */
	RX_REQUIRE(!drawable_update(&table[1].dw, 0));
	RX_REQUIRE(table[1].inv.update);
	RX_REQUIRE(!table[1].inv.draw);
	RX_REQUIRE(!table[1].inv.end);
	RX_REQUIRE(!table[1].inv.finish);
}

RX_TEST_CASE(basics, draw)
{
	struct invokes inv = {0};
	struct drawable dw = INIT(&inv, my_update_true);

	drawable_draw(&dw);

	RX_REQUIRE(!inv.update);
	RX_REQUIRE(inv.draw);
	RX_REQUIRE(!inv.end);
	RX_REQUIRE(!inv.finish);
}

RX_TEST_CASE(basics, end)
{
	struct invokes inv = {0};
	struct drawable dw = INIT(&inv, my_update_true);

	drawable_end(&dw);

	RX_REQUIRE(!inv.update);
	RX_REQUIRE(!inv.draw);
	RX_REQUIRE(inv.end);
	RX_REQUIRE(!inv.finish);
}

RX_TEST_CASE(basics, finish)
{
	struct invokes inv = {0};
	struct drawable dw = INIT(&inv, my_update_true);

	drawable_finish(&dw);

	RX_REQUIRE(!inv.update);
	RX_REQUIRE(!inv.draw);
	RX_REQUIRE(!inv.end);
	RX_REQUIRE(inv.finish);
}

RX_TEST_CASE(stack, add)
{
	struct drawable_stack st = {0};
	struct drawable dw = {0};

	RX_INT_REQUIRE_EQUAL(drawable_stack_add(&st, &dw), 0);

	/* Now fill up. */
	for (int i = 0; i < DRAWABLE_STACK_MAX - 1; ++i)
		RX_INT_REQUIRE_EQUAL(drawable_stack_add(&st, &dw), 0);

	/* This one should not fit in. */
	RX_INT_REQUIRE_EQUAL(drawable_stack_add(&st, &dw), -1);
}

RX_TEST_CASE(stack, update)
{
	struct {
		struct invokes inv;
		struct drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_false)       },
		{ .dw = INIT(&table[1], my_update_true)        },
		{ .dw = INIT(&table[2], my_update_false)       },
		{ .dw = INIT(&table[3], my_update_false)       },
		{ .dw = INIT(&table[4], my_update_true)        },
		{ .dw = INIT(&table[5], my_update_true)        },
		{ .dw = INIT(&table[6], my_update_false)	},
	};

	struct drawable_stack st = {0};

	drawable_stack_add(&st, &table[0].dw);
	drawable_stack_add(&st, &table[1].dw);
	drawable_stack_add(&st, &table[2].dw);
	drawable_stack_add(&st, &table[3].dw);
	drawable_stack_add(&st, &table[4].dw);
	drawable_stack_add(&st, &table[5].dw);
	drawable_stack_add(&st, &table[6].dw);

	RX_REQUIRE(!drawable_stack_update(&st, 0));

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
	RX_PTR_REQUIRE_EQUAL(st.objects[0], &table[0].dw);
	RX_PTR_REQUIRE_EQUAL(st.objects[2], &table[2].dw);
	RX_PTR_REQUIRE_EQUAL(st.objects[3], &table[3].dw);
	RX_PTR_REQUIRE_EQUAL(st.objects[6], &table[6].dw);

	/* The following must have been NULL-ed. */
	RX_PTR_REQUIRE_EQUAL(st.objects[1], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[4], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[5], NULL);
	
	/*
	 * Now make all actions to return 1 and check if it cleans the stack.
	 */
	table[0].dw.update =
	    table[2].dw.update =
	    table[3].dw.update =
	    table[6].dw.update = my_update_true;

	RX_REQUIRE(drawable_stack_update(&st, 0));
	RX_PTR_REQUIRE_EQUAL(st.objects[0], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[1], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[2], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[3], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[4], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[5], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[6], NULL);
}

RX_TEST_CASE(stack, finish)
{
	struct {
		struct invokes inv;
		struct drawable dw;
	} table[] = {
		{ .dw = INIT(&table[0], my_update_true)        },
		{ .dw = INIT(&table[0], my_update_false)       },
	};
	struct drawable_stack st = {0};

	drawable_stack_add(&st, &table[0].dw);
	drawable_stack_add(&st, &table[1].dw);
	drawable_stack_finish(&st);

	RX_REQUIRE(!table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(table[0].inv.end);
	RX_REQUIRE(table[0].inv.finish);

	RX_REQUIRE(!table[0].inv.update);
	RX_REQUIRE(!table[0].inv.draw);
	RX_REQUIRE(table[0].inv.end);
	RX_REQUIRE(table[0].inv.finish);

	/* They should also be NULL'ed. */
	RX_PTR_REQUIRE_EQUAL(st.objects[0], NULL);
	RX_PTR_REQUIRE_EQUAL(st.objects[1], NULL);
}

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

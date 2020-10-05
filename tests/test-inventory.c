/*
 * test-inventory.c -- test inventory functions
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

#include <core/item.h>
#include <core/inventory.h>

static struct item potion = {
	.name = "Potion",
	.stackable = 64
};

static struct item elixir = {
	.name = "Elixir",
	.stackable = 64
};

static struct item sword = {
	.name = "Sword",
	.stackable = 1
};

#define SET(iv, r, c, i, a)                                             \
do {                                                                    \
        (iv)->items[(r)][(c)].item = (i);                               \
        (iv)->items[(r)][(c)].amount = (a);                             \
} while (0)                                                             \

GREATEST_TEST
push_simple(void)
{
	struct inventory inv = { 0 };
	unsigned int left;

	/*
	 * [0][0] should contain 50 potions.
	 */
	left = inventory_push(&inv, &potion, 50U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 50U);
	inventory_clear(&inv);

	/*
	 * [0][1] should contain 64 potions.
	 * [0][2] should contain 10 potions.
	 */
	left = inventory_push(&inv, &potion, 74U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 64U);
	GREATEST_ASSERT_EQ(inv.items[0][1].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][1].amount, 10U);
	inventory_clear(&inv);

	/*
	 * 1. We push 10 potions
	 * 2. We push 10 more potions.
	 *
	 * [0][0] should contain 20 potions.
	 */
	left = inventory_push(&inv, &potion, 10U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 10U);
	left = inventory_push(&inv, &potion, 10U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 20U);
	inventory_clear(&inv);

	GREATEST_PASS();
}

GREATEST_TEST
push_full(void)
{
	struct inventory inv = { 0 };
	unsigned int left;

	/*
	 * Fill the inventory with
	 *
	 * - 29 x 64 potions
	 * - Last slot will have already 50 potions.
	 *
	 * This means the maximum potions to push is only 14.
	 */
	for (int i = 0; i < 29; ++i) {
		left = inventory_push(&inv, &potion, 64U);
		GREATEST_ASSERT_EQ(left, 0U);
	}

	left = inventory_push(&inv, &potion, 50U);
	GREATEST_ASSERT_EQ(left, 0U);

	/*
	 * Try to add 20 potions, only 14 should be pushed.
	 */
	left = inventory_push(&inv, &potion, 20U);
	GREATEST_ASSERT_EQ(left, 6U);
	GREATEST_ASSERT_EQ(inv.items[2][9].amount, 64U);

	GREATEST_PASS();
}

GREATEST_TEST
push_other(void)
{
	struct inventory inv = { 0 };
	unsigned int left;

	/*
	 * [0][0] should contain 30 potions.
	 */
	left = inventory_push(&inv, &potion, 30U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 30U);

	/*
	 * Since elixir is not a potion, it should be pushed into an other
	 * cell.
	 *
	 * [0][0] should be untouched
	 * [0][1] should contain 30 elixir.
	 */
	left = inventory_push(&inv, &elixir, 30U);
	GREATEST_ASSERT_EQ(left, 0U);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 30U);
	GREATEST_ASSERT_EQ(inv.items[0][1].item, &elixir);
	GREATEST_ASSERT_EQ(inv.items[0][1].amount, 30U);

	GREATEST_PASS();
}

GREATEST_SUITE(push)
{
	GREATEST_RUN_TEST(push_simple);
	GREATEST_RUN_TEST(push_full);
	GREATEST_RUN_TEST(push_other);
}

GREATEST_TEST
sort_simple(void)
{
	struct inventory inv = { 0 };

	/*
	 * Create this inventory:
	 *
	 *   0      1      2      3      4      5      6      7      8      9
	 * 0 [    ] [P:10] [    ] [    ] [S: 1] [    ] [    ] [E:24] [    ] [    ]
	 * 1 [    ] [    ] [P:12] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 * 2 [    ] [    ] [    ] [    ] [E: 8] [    ] [    ] [    ] [P: 5] [    ]
	 */
	SET(&inv, 0, 1, &potion, 10);
	SET(&inv, 0, 4, &sword, 1);
	SET(&inv, 0, 7, &elixir, 24);
	SET(&inv, 1, 2, &potion, 12);
	SET(&inv, 2, 4, &elixir, 8);
	SET(&inv, 2, 8, &potion, 5);

	/*
	 * Should look like this:
	 *
	 *   0      1      2      3      4      5      6      7      8      9
	 * 0 [E:32] [P:27] [S: 1] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 * 1 [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 * 2 [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 */
	inventory_sort(&inv);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &elixir);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 32U);
	GREATEST_ASSERT_EQ(inv.items[0][1].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][1].amount, 27U);
	GREATEST_ASSERT_EQ(inv.items[0][2].item, &sword);
	GREATEST_ASSERT_EQ(inv.items[0][2].amount, 1U);

	GREATEST_PASS();
}

GREATEST_TEST
sort_with_full(void)
{
	struct inventory inv = { 0 };

	/*
	 * Create this inventory:
	 *
	 *   0      1      2      3      4      5      6      7      8      9
	 * 0 [    ] [P:62] [    ] [    ] [S: 1] [    ] [    ] [E:64] [    ] [    ]
	 * 1 [    ] [    ] [P:12] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 * 2 [    ] [    ] [    ] [    ] [E: 8] [    ] [    ] [    ] [P: 5] [    ]
	 */
	SET(&inv, 0, 1, &potion, 62);
	SET(&inv, 0, 4, &sword, 1);
	SET(&inv, 0, 7, &elixir, 64);
	SET(&inv, 1, 2, &potion, 12);
	SET(&inv, 2, 4, &elixir, 8);
	SET(&inv, 2, 8, &potion, 5);

	/*
	 * Should look like this:
	 *
	 *   0      1      2      3      4      5      6      7      8      9
	 * 0 [E:64] [E: 8] [P:64] [P:10] [S: 1] [    ] [    ] [    ] [    ] [    ]
	 * 1 [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 * 2 [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ] [    ]
	 */
	inventory_sort(&inv);
	GREATEST_ASSERT_EQ(inv.items[0][0].item, &elixir);
	GREATEST_ASSERT_EQ(inv.items[0][0].amount, 64U);
	GREATEST_ASSERT_EQ(inv.items[0][1].item, &elixir);
	GREATEST_ASSERT_EQ(inv.items[0][1].amount, 8U);
	GREATEST_ASSERT_EQ(inv.items[0][2].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][2].amount, 64U);
	GREATEST_ASSERT_EQ(inv.items[0][3].item, &potion);
	GREATEST_ASSERT_EQ(inv.items[0][3].amount, 15U);
	GREATEST_ASSERT_EQ(inv.items[0][4].item, &sword);
	GREATEST_ASSERT_EQ(inv.items[0][4].amount, 1U);

	GREATEST_PASS();
}

GREATEST_SUITE(sort)
{
	GREATEST_RUN_TEST(sort_simple);
	GREATEST_RUN_TEST(sort_with_full);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(push);
	GREATEST_RUN_SUITE(sort);
	GREATEST_MAIN_END();
}

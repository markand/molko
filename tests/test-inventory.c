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

#include "item.h"
#include "inventory.h"

static struct item potion = {
	.name = "Potion",
	.stackable = 64
};

static struct item elixir = {
	.name = "Elixir",
	.stackable = 64
};

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

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(push);
	GREATEST_MAIN_END();
}

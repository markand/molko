/*
 * test-property.c -- test properties
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

#include <greatest.h>

#include <property.h>

TEST
add(void)
{
	struct property_list list = { 0 };

	/*
	 * 1 value:
	 * - 0: "name" = "Jean Dujardin"
	 */
	property_add(&list, property_new("name", "Jean Dujardin"));
	ASSERT_STR_EQ(list.next->key, "name");
	ASSERT_STR_EQ(list.next->value, "Jean Dujardin");
	ASSERT(!list.next->next);

	/*
	 * 1 value:
	 * - 0: "name" = "Jean Dujardin"
	 * - 1: "name" = "Daniel Balavoine"
	 */
	property_add(&list, property_new("name", "Daniel Balavoine"));
	ASSERT_STR_EQ(list.next->key, "name");
	ASSERT_STR_EQ(list.next->value, "Jean Dujardin");
	ASSERT_STR_EQ(list.next->next->key, "name");
	ASSERT_STR_EQ(list.next->next->value, "Daniel Balavoine");
	ASSERT(!list.next->next->next);

	property_clear(&list);
	PASS();
}

TEST
find(void)
{
	struct property_list list = { 0 };

	/*
	 * 2 value:
	 * - "start-x" = "10"
	 * - "start-y" = "20"
	 */
	property_add(&list, property_new("start-x", "10"));
	property_add(&list, property_new("start-y", "20"));
	ASSERT_STR_EQ("start-x", property_find(&list, "start-x")->key);
	ASSERT_STR_EQ("10", property_find(&list, "start-x")->value);
	ASSERT_STR_EQ("start-y", property_find(&list, "start-y")->key);
	ASSERT_STR_EQ("20", property_find(&list, "start-y")->value);
	property_clear(&list);
	PASS();
}

TEST
clear(void)
{
	struct property_list list = { 0 };

	property_add(&list, property_new("start-x", "10"));
	property_add(&list, property_new("start-y", "20"));
	property_clear(&list);
	ASSERT(!list.next);
	PASS();
}

SUITE(simple)
{
	RUN_TEST(add);
	RUN_TEST(find);
	RUN_TEST(clear);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(simple);
	GREATEST_MAIN_END();
}

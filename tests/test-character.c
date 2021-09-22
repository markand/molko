/*
 * test-character.c -- test character routines
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

#include <stdio.h>
#include <string.h>

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <rpg/character.h>
#include <rpg/save.h>

static void
clean(void *data)
{
	(void)data;

	remove("test.db");
}

GREATEST_TEST
test_save_simple(void)
{
	struct save db;
	struct character ch = {
		.name = "david",
		.hp = 1989,
		.mp = 1,
		.level = 18,
		.team_order = 1,
		.hpbonus = 500,
		.mpbonus = 50,
		.atkbonus = 1001,
		.defbonus = 1002,
		.agtbonus = 1003,
		.luckbonus = 1004
	};

	GREATEST_ASSERT(save_open_path(&db, "test.db", SAVE_MODE_WRITE) == 0);
	GREATEST_ASSERT(character_save(&ch, &db));

	/* Restore. */
	memset(&ch, 0, sizeof (ch));
	ch.name = "david";

	GREATEST_ASSERT(character_load(&ch, &db) == 0);
	GREATEST_ASSERT_EQ(1989, ch.hp);
	GREATEST_ASSERT_EQ(1, ch.mp);
	GREATEST_ASSERT_EQ(18, ch.level);
	GREATEST_ASSERT_EQ(1, ch.team_order);
	GREATEST_ASSERT_EQ(500, ch.hpbonus);
	GREATEST_ASSERT_EQ(50, ch.mpbonus);
	GREATEST_ASSERT_EQ(1001, ch.atkbonus);
	GREATEST_ASSERT_EQ(1002, ch.defbonus);
	GREATEST_ASSERT_EQ(1003, ch.agtbonus);
	GREATEST_ASSERT_EQ(1004, ch.luckbonus);
	GREATEST_PASS();
}

GREATEST_SUITE(suite_save)
{
	GREATEST_SET_SETUP_CB(clean, NULL);
	GREATEST_SET_TEARDOWN_CB(clean, NULL);
	GREATEST_RUN_TEST(test_save_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_save);
	GREATEST_MAIN_END();
}

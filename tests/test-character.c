/*
 * test-character.c -- test character routines
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

#include <stdio.h>
#include <string.h>

#include <mlk/rpg/character.h>
#include <mlk/rpg/save.h>

#include <dt.h>

static void
test_basics_load(void)
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

	DT_EQ_INT(save_open_path(&db, "test.db", SAVE_MODE_WRITE), 0);
	DT_EQ_INT(character_save(&ch, &db), 0);

	/* Restore. */
	memset(&ch, 0, sizeof (ch));
	ch.name = "david";

	DT_ASSERT(character_load(&ch, &db) == 0);
	DT_EQ_INT(ch.hp, 1989);
	DT_EQ_INT(ch.mp, 1);
	DT_EQ_INT(ch.level, 18);
	DT_EQ_INT(ch.team_order, 1);
	DT_EQ_INT(ch.hpbonus, 500);
	DT_EQ_INT(ch.mpbonus, 50);
	DT_EQ_INT(ch.atkbonus, 1001);
	DT_EQ_INT(ch.defbonus, 1002);
	DT_EQ_INT(ch.agtbonus, 1003);
	DT_EQ_INT(ch.luckbonus, 1004);
}

int
main(void)
{
	remove("test.db");

	DT_RUN(test_basics_load);
	DT_SUMMARY();

	remove("test.db");
}

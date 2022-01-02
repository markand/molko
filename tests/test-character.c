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

#include <rexo.h>

#include <rpg/character.h>
#include <rpg/save.h>

RX_SET_UP(setup)
{
	remove("test.db");

	return RX_SUCCESS;
}

RX_TEAR_DOWN(teardown)
{
	remove("test.db");
}

RX_FIXTURE(basics_fixture, void *, .set_up = setup, .tear_down = teardown);

RX_TEST_CASE(test, save_simple, .fixture = basics_fixture)
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

	RX_INT_REQUIRE_EQUAL(save_open_path(&db, "test.db", SAVE_MODE_WRITE), 0);
	RX_INT_REQUIRE_EQUAL(character_save(&ch, &db), 0);

	/* Restore. */
	memset(&ch, 0, sizeof (ch));
	ch.name = "david";

	RX_REQUIRE(character_load(&ch, &db) == 0);
	RX_INT_REQUIRE_EQUAL(ch.hp, 1989);
	RX_INT_REQUIRE_EQUAL(ch.mp, 1);
	RX_INT_REQUIRE_EQUAL(ch.level, 18);
	RX_INT_REQUIRE_EQUAL(ch.team_order, 1);
	RX_INT_REQUIRE_EQUAL(ch.hpbonus, 500);
	RX_INT_REQUIRE_EQUAL(ch.mpbonus, 50);
	RX_INT_REQUIRE_EQUAL(ch.atkbonus, 1001);
	RX_INT_REQUIRE_EQUAL(ch.defbonus, 1002);
	RX_INT_REQUIRE_EQUAL(ch.agtbonus, 1003);
	RX_INT_REQUIRE_EQUAL(ch.luckbonus, 1004);
}

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

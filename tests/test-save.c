/*
 * test-save.c -- test save routines
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

#include <rexo.h>

#include <rpg/property.h>
#include <rpg/save.h>

RX_TEAR_DOWN(teardown)
{
	remove("1.db");
	remove("2.db");
}

RX_FIXTURE(basics_fixture, void *, .tear_down = teardown);

RX_TEST_CASE(open, read, .fixture = basics_fixture)
{
	struct save db;

	/* Non-existent should return error. */
	RX_INT_REQUIRE_EQUAL(save_open_path(&db, "1.db", SAVE_MODE_READ), -1);
	save_finish(&db);
}

RX_TEST_CASE(open, write, .fixture = basics_fixture)
{
	struct save db[2] = {0};

	/* Write should work on both non-existent and existent database. */
	RX_INT_REQUIRE_EQUAL(save_open_path(&db[0], "1.db", SAVE_MODE_WRITE), 0);
	RX_INT_REQUIRE_EQUAL(save_open_path(&db[1], "2.db", SAVE_MODE_WRITE), 0);

	/* Update and create date must not be 0. */
	RX_REQUIRE(db[0].created > 0);
	RX_REQUIRE(db[0].updated > 0);
	RX_REQUIRE(db[1].created > 0);
	RX_REQUIRE(db[1].updated > 0);

	save_finish(&db[0]);
	save_finish(&db[1]);

	/* Should work again. */
	RX_REQUIRE(save_open_path(&db[0], "1.db", SAVE_MODE_WRITE) == 0);
	RX_REQUIRE(save_open_path(&db[1], "2.db", SAVE_MODE_WRITE) == 0);
	RX_REQUIRE(db[0].created > 0);
	RX_REQUIRE(db[0].updated > 0);
	RX_REQUIRE(db[1].created > 0);
	RX_REQUIRE(db[1].updated > 0);

	save_finish(&db[0]);
	save_finish(&db[1]);
}

#if 0

RX_TEST_CASE(properties, set)
{
	struct save db;
	struct property prop;

	RX_REQUIRE(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);

	/* Insert a new property 'state'. */
	prop = (struct property){.key = "state", .value = "intro"};
	RX_REQUIRE(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	RX_REQUIRE(property_load(&prop, &db) == 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "intro");

	/* Now we replace the value. */
	prop = (struct property){.key = "state", .value = "map"};
	RX_REQUIRE(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	RX_REQUIRE(property_load(&prop, &db) == 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "map");

	save_finish(&db);

	GREATEST_PASS();
}

GREATEST_TEST
properties_notfound(void)
{
	struct save db;
	struct property prop = {.key = "state"};

	RX_REQUIRE(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);
	RX_REQUIRE(property_load(&prop, &db) < 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "");

	GREATEST_PASS();
}

GREATEST_TEST
properties_remove(void)
{
	struct save db;
	struct property prop;

	RX_REQUIRE(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);

	/* Insert a new property 'initialized'. */
	prop = (struct property){.key = "state", .value = "intro"};
	RX_REQUIRE(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	RX_REQUIRE(property_remove(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	RX_REQUIRE(property_load(&prop, &db) < 0);
	RX_STR_REQUIRE_EQUAL(prop.value, "");

	GREATEST_PASS();
}

#endif

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

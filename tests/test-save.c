/*
 * test-save.c -- test save routines
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

#include <stdio.h>

#include <mlk/rpg/property.h>
#include <mlk/rpg/save.h>

#include <dt.h>

static void
cleanup(void)
{
	remove("1.db");
	remove("2.db");
}

static void
test_basics_read(void)
{
	struct mlk_save db;

	/* Non-existent should return error. */
	DT_EQ_INT(mlk_save_open_path(&db, "1.db", MLK_SAVE_MODE_READ), -1);
	mlk_save_finish(&db);
	cleanup();
}

static void
test_basics_write(void)
{
	struct mlk_save db[2] = {0};

	/* Write should work on both non-existent and existent database. */
	DT_EQ_INT(mlk_save_open_path(&db[0], "1.db", MLK_SAVE_MODE_WRITE), 0);
	DT_EQ_INT(mlk_save_open_path(&db[1], "2.db", MLK_SAVE_MODE_WRITE), 0);

	/* Update and create date must not be 0. */
	DT_ASSERT(db[0].created > 0);
	DT_ASSERT(db[0].updated > 0);
	DT_ASSERT(db[1].created > 0);
	DT_ASSERT(db[1].updated > 0);

	mlk_save_finish(&db[0]);
	mlk_save_finish(&db[1]);

	/* Should work again. */
	DT_ASSERT(mlk_save_open_path(&db[0], "1.db", MLK_SAVE_MODE_WRITE) == 0);
	DT_ASSERT(mlk_save_open_path(&db[1], "2.db", MLK_SAVE_MODE_WRITE) == 0);
	DT_ASSERT(db[0].created > 0);
	DT_ASSERT(db[0].updated > 0);
	DT_ASSERT(db[1].created > 0);
	DT_ASSERT(db[1].updated > 0);

	mlk_save_finish(&db[0]);
	mlk_save_finish(&db[1]);
}

#if 0

RX_TEST_CASE(properties, set)
{
	struct save db;
	struct property prop;

	DT_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);

	/* Insert a new property 'state'. */
	prop = (struct property){.key = "state", .value = "intro"};
	DT_ASSERT(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	DT_ASSERT(property_load(&prop, &db) == 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "intro");

	/* Now we replace the value. */
	prop = (struct property){.key = "state", .value = "map"};
	DT_ASSERT(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	DT_ASSERT(property_load(&prop, &db) == 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "map");

	save_finish(&db);

	GREATEST_PASS();
}

GREATEST_TEST
properties_notfound(void)
{
	struct save db;
	struct property prop = {.key = "state"};

	DT_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);
	DT_ASSERT(property_load(&prop, &db) < 0);
	GREATEST_ASSERT_STR_EQ(prop.value, "");

	GREATEST_PASS();
}

GREATEST_TEST
properties_remove(void)
{
	struct save db;
	struct property prop;

	DT_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE) == 0);

	/* Insert a new property 'initialized'. */
	prop = (struct property){.key = "state", .value = "intro"};
	DT_ASSERT(property_save(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	DT_ASSERT(property_remove(&prop, &db) == 0);
	prop = (struct property){.key = "state"};
	DT_ASSERT(property_load(&prop, &db) < 0);
	DT_EQ_STR(prop.value, "");

	GREATEST_PASS();
}

#endif

int
main(void)
{
	DT_RUN_EX(test_basics_read, cleanup, cleanup);
	DT_RUN_EX(test_basics_write, cleanup, cleanup);
	DT_SUMMARY();
}

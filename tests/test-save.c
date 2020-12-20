/*
 * test-save.c -- test save routines
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

#include <stdio.h>

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <rpg/save.h>

static void
clean(void *data)
{
	(void)data;

	remove("1.db");
	remove("2.db");
}

GREATEST_TEST
open_read(void)
{
	struct save db;

	/* Non-existent should return false. */
	GREATEST_ASSERT(!save_open_path(&db, "1.db", SAVE_MODE_READ));

	save_finish(&db);

	GREATEST_PASS();
}

GREATEST_TEST
open_write(void)
{
	struct save db[2] = {0};

	/* Write should work on both non-existent and existent database. */
	GREATEST_ASSERT(save_open_path(&db[0], "1.db", SAVE_MODE_WRITE));
	GREATEST_ASSERT(save_open_path(&db[1], "2.db", SAVE_MODE_WRITE));

	/* Update and create date must not be 0. */
	GREATEST_ASSERT(db[0].created > 0);
	GREATEST_ASSERT(db[0].updated > 0);
	GREATEST_ASSERT(db[1].created > 0);
	GREATEST_ASSERT(db[1].updated > 0);

	save_finish(&db[0]);
	save_finish(&db[1]);

	/* Should work again. */
	GREATEST_ASSERT(save_open_path(&db[0], "1.db", SAVE_MODE_WRITE));
	GREATEST_ASSERT(save_open_path(&db[1], "2.db", SAVE_MODE_WRITE));
	GREATEST_ASSERT(db[0].created > 0);
	GREATEST_ASSERT(db[0].updated > 0);
	GREATEST_ASSERT(db[1].created > 0);
	GREATEST_ASSERT(db[1].updated > 0);

	save_finish(&db[0]);
	save_finish(&db[1]);

	GREATEST_PASS();
}

GREATEST_SUITE(open_suite)
{
	GREATEST_SET_SETUP_CB(clean, NULL);
	GREATEST_SET_TEARDOWN_CB(clean, NULL);
	GREATEST_RUN_TEST(open_read);
	GREATEST_RUN_TEST(open_write);
}

GREATEST_TEST
properties_set(void)
{
	struct save db;
	struct save_property prop;

	GREATEST_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE));

	/* Insert a new property 'state'. */
	prop = (struct save_property){.key = "state", .value = "intro"};
	GREATEST_ASSERT(save_set_property(&db, &prop));
	prop = (struct save_property){.key = "state"};
	GREATEST_ASSERT(save_get_property(&db, &prop));
	GREATEST_ASSERT_STR_EQ(prop.value, "intro");

	/* Now we replace the value. */
	prop = (struct save_property){.key = "state", .value = "map"};
	GREATEST_ASSERT(save_set_property(&db, &prop));
	prop = (struct save_property){.key = "state"};
	GREATEST_ASSERT(save_get_property(&db, &prop));
	GREATEST_ASSERT_STR_EQ(prop.value, "map");

	save_finish(&db);

	GREATEST_PASS();
}

GREATEST_TEST
properties_notfound(void)
{
	struct save db;
	struct save_property prop = {.key = "state"};

	GREATEST_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE));
	GREATEST_ASSERT(!save_get_property(&db, &prop));
	GREATEST_ASSERT_STR_EQ(prop.value, "");

	GREATEST_PASS();
}

GREATEST_TEST
properties_remove(void)
{
	struct save db;
	struct save_property prop;

	GREATEST_ASSERT(save_open_path(&db, "1.db", SAVE_MODE_WRITE));

	/* Insert a new property 'initialized'. */
	prop = (struct save_property){.key = "state", .value = "intro"};
	GREATEST_ASSERT(save_set_property(&db, &prop));
	prop = (struct save_property){.key = "state"};
	GREATEST_ASSERT(save_remove_property(&db, &prop));
	prop = (struct save_property){.key = "state"};
	GREATEST_ASSERT(!save_get_property(&db, &prop));
	GREATEST_ASSERT_STR_EQ(prop.value, "");

	GREATEST_PASS();
}

GREATEST_SUITE(properties_suite)
{
	GREATEST_SET_SETUP_CB(clean, NULL);
	GREATEST_SET_TEARDOWN_CB(clean, NULL);
	GREATEST_RUN_TEST(properties_set);
	GREATEST_RUN_TEST(properties_notfound);
	GREATEST_RUN_TEST(properties_remove);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(open_suite);
	GREATEST_RUN_SUITE(properties_suite);
	GREATEST_MAIN_END();
}

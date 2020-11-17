/*
 * test-map.c -- test map routines
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

#include <core/core.h>
#include <core/error.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/window.h>

#include <rpg/map-file.h>
#include <rpg/map.h>

GREATEST_TEST
test_basics_sample(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	GREATEST_ASSERT(map_file_open(&loader, &map, DIRECTORY "sample-map.map"));
	GREATEST_ASSERT_STR_EQ("This is a test map", map.title);
	GREATEST_ASSERT_EQ(4, map.columns);
	GREATEST_ASSERT_EQ(2, map.rows);

	GREATEST_ASSERT_EQ(0, map.layers[0].tiles[0]);
	GREATEST_ASSERT_EQ(1, map.layers[0].tiles[1]);
	GREATEST_ASSERT_EQ(2, map.layers[0].tiles[2]);
	GREATEST_ASSERT_EQ(3, map.layers[0].tiles[3]);
	GREATEST_ASSERT_EQ(4, map.layers[0].tiles[4]);
	GREATEST_ASSERT_EQ(5, map.layers[0].tiles[5]);
	GREATEST_ASSERT_EQ(6, map.layers[0].tiles[6]);
	GREATEST_ASSERT_EQ(7, map.layers[0].tiles[7]);

	GREATEST_ASSERT_EQ(8,  map.layers[1].tiles[0]);
	GREATEST_ASSERT_EQ(9,  map.layers[1].tiles[1]);
	GREATEST_ASSERT_EQ(10, map.layers[1].tiles[2]);
	GREATEST_ASSERT_EQ(11, map.layers[1].tiles[3]);
	GREATEST_ASSERT_EQ(12, map.layers[1].tiles[4]);
	GREATEST_ASSERT_EQ(13, map.layers[1].tiles[5]);
	GREATEST_ASSERT_EQ(14, map.layers[1].tiles[6]);
	GREATEST_ASSERT_EQ(15, map.layers[1].tiles[7]);

	GREATEST_ASSERT_EQ(64U, map.tileset->sprite->cellw);
	GREATEST_ASSERT_EQ(32U, map.tileset->sprite->cellh);

	GREATEST_ASSERT_EQ(4, map.tileset->tiledefsz);

	GREATEST_ASSERT_EQ(129, map.tileset->tiledefs[0].id);
	GREATEST_ASSERT_EQ(8, map.tileset->tiledefs[0].x);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[0].y);
	GREATEST_ASSERT_EQ(56, map.tileset->tiledefs[0].w);
	GREATEST_ASSERT_EQ(40, map.tileset->tiledefs[0].h);

	GREATEST_ASSERT_EQ(130, map.tileset->tiledefs[1].id);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[1].x);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[1].y);
	GREATEST_ASSERT_EQ(62, map.tileset->tiledefs[1].w);
	GREATEST_ASSERT_EQ(40, map.tileset->tiledefs[1].h);

	GREATEST_ASSERT_EQ(132, map.tileset->tiledefs[2].id);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[2].x);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[2].y);
	GREATEST_ASSERT_EQ(64, map.tileset->tiledefs[2].w);
	GREATEST_ASSERT_EQ(40, map.tileset->tiledefs[2].h);

	GREATEST_ASSERT_EQ(133, map.tileset->tiledefs[3].id);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[3].x);
	GREATEST_ASSERT_EQ(0, map.tileset->tiledefs[3].y);
	GREATEST_ASSERT_EQ(58, map.tileset->tiledefs[3].w);
	GREATEST_ASSERT_EQ(40, map.tileset->tiledefs[3].h);

	map_finish(&map);
	map_file_finish(&loader);

	GREATEST_PASS();
}

GREATEST_TEST
test_error_title(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	GREATEST_ASSERT(!map_file_open(&loader, &map, DIRECTORY "error-title.map"));

	map_finish(&map);
	map_file_finish(&loader);

	GREATEST_PASS();
}

GREATEST_TEST
test_error_columns(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	GREATEST_ASSERT(!map_file_open(&loader, &map, DIRECTORY "error-columns.map"));

	map_finish(&map);
	map_file_finish(&loader);

	GREATEST_PASS();
}

GREATEST_TEST
test_error_rows(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	GREATEST_ASSERT(!map_file_open(&loader, &map, DIRECTORY "error-rows.map"));

	map_finish(&map);
	map_file_finish(&loader);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(test_basics_sample);
}

GREATEST_SUITE(suite_errors)
{
	GREATEST_RUN_TEST(test_error_title);
	GREATEST_RUN_TEST(test_error_columns);
	GREATEST_RUN_TEST(test_error_rows);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();

	/*
	 * This test opens graphical images and therefore need to initialize a
	 * window and all of the API. As tests sometime run on headless machine
	 * we will skip if it fails to initialize.
	 */

	if (core_init() && window_open("test-map", 100, 100)) {
		GREATEST_RUN_SUITE(suite_basics);
		GREATEST_RUN_SUITE(suite_errors);
	}

	GREATEST_MAIN_END();

	return 0;
}

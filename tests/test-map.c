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

#include <core/error.h>
#include <core/map.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/window.h>

#include <assets/maps/sample-map.h>
#include <assets/maps/error-title.h>
#include <assets/maps/error-width.h>
#include <assets/maps/error-height.h>
#include <assets/maps/error-tilewidth.h>
#include <assets/maps/error-tileheight.h>

GREATEST_TEST
test_sample(void)
{
	struct map_data map;

	GREATEST_ASSERT(map_data_openmem(&map, maps_sample_map, sizeof (maps_sample_map)));
	GREATEST_ASSERT_STR_EQ("This is a test map", map.title);
	GREATEST_ASSERT_EQ(2, map.w);
	GREATEST_ASSERT_EQ(2, map.h);
	GREATEST_ASSERT_EQ(32, map.tile_w);
	GREATEST_ASSERT_EQ(16, map.tile_h);
	GREATEST_ASSERT_EQ(0, map.layers[0].tiles[0]);
	GREATEST_ASSERT_EQ(1, map.layers[0].tiles[1]);
	GREATEST_ASSERT_EQ(2, map.layers[0].tiles[2]);
	GREATEST_ASSERT_EQ(3, map.layers[0].tiles[3]);
	GREATEST_ASSERT_EQ(4, map.layers[1].tiles[0]);
	GREATEST_ASSERT_EQ(5, map.layers[1].tiles[1]);
	GREATEST_ASSERT_EQ(6, map.layers[1].tiles[2]);
	GREATEST_ASSERT_EQ(7, map.layers[1].tiles[3]);
	GREATEST_PASS();
}

GREATEST_TEST
test_error_title(void)
{
	struct map_data map;

	GREATEST_ASSERT(!map_data_openmem(&map, maps_error_title, sizeof (maps_error_title)));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_width(void)
{
	struct map_data map;

	GREATEST_ASSERT(!map_data_openmem(&map, maps_error_width, sizeof (maps_error_width)));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_height(void)
{
	struct map_data map;

	GREATEST_ASSERT(!map_data_openmem(&map, maps_error_height, sizeof (maps_error_height)));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_tilewidth(void)
{
	struct map_data map;

	GREATEST_ASSERT(!map_data_openmem(&map, maps_error_tilewidth, sizeof (maps_error_tilewidth)));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_tileheight(void)
{
	struct map_data map;

	GREATEST_ASSERT(!map_data_openmem(&map, maps_error_tileheight, sizeof (maps_error_tileheight)));
	GREATEST_PASS();
}

GREATEST_SUITE(basics)
{
	GREATEST_RUN_TEST(test_sample);
}

GREATEST_SUITE(errors)
{
	GREATEST_RUN_TEST(test_error_title);
	GREATEST_RUN_TEST(test_error_width);
	GREATEST_RUN_TEST(test_error_height);
	GREATEST_RUN_TEST(test_error_tilewidth);
	GREATEST_RUN_TEST(test_error_tileheight);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(basics);
	GREATEST_RUN_SUITE(errors);
	GREATEST_MAIN_END();

	return 0;
}

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

#include <greatest.h>

#include <error.h>
#include <map.h>
#include <sys.h>
#include <window.h>

static void
setup(void *data)
{
	(void)data;

	if (!sys_init())
		panic();
	if (!window_init("Test map", 100, 100))
		panic();
}

static void
teardown(void *data)
{
	(void)data;

	window_close();
	sys_close();
}

TEST
sample(void)
{
	struct map map;

	ASSERT(map_open(&map, sys_datapath("maps/sample-map.map")));
	ASSERT_STR_EQ("This is a test map", map.title);
	ASSERT_EQ(2, map.width);
	ASSERT_EQ(2, map.height);
	ASSERT_EQ(32, map.tilewidth);
	ASSERT_EQ(16, map.tileheight);
	ASSERT_EQ(0, map.layers[0].tiles[0]);
	ASSERT_EQ(1, map.layers[0].tiles[1]);
	ASSERT_EQ(2, map.layers[0].tiles[2]);
	ASSERT_EQ(3, map.layers[0].tiles[3]);
	ASSERT_EQ(4, map.layers[1].tiles[0]);
	ASSERT_EQ(5, map.layers[1].tiles[1]);
	ASSERT_EQ(6, map.layers[1].tiles[2]);
	ASSERT_EQ(7, map.layers[1].tiles[3]);
	PASS();
}

TEST
error_title(void)
{
	struct map map;

	ASSERT(!map_open(&map, sys_datapath("maps/error-title.map")));
	PASS();
}

TEST
error_width(void)
{
	struct map map;

	ASSERT(!map_open(&map, sys_datapath("maps/error-width.map")));
	PASS();
}

TEST
error_height(void)
{
	struct map map;

	ASSERT(!map_open(&map, sys_datapath("maps/error-height.map")));
	PASS();
}

TEST
error_tilewidth(void)
{
	struct map map;

	ASSERT(!map_open(&map, sys_datapath("maps/error-tilewidth.map")));
	PASS();
}

TEST
error_tileheight(void)
{
	struct map map;

	ASSERT(!map_open(&map, sys_datapath("maps/error-tileheight.map")));
	PASS();
}

SUITE(basics)
{
	SET_SETUP(setup, NULL);
	SET_TEARDOWN(teardown, NULL);
	RUN_TEST(sample);
}

SUITE(errors)
{
	SET_SETUP(setup, NULL);
	SET_TEARDOWN(teardown, NULL);
	RUN_TEST(error_title);
	RUN_TEST(error_width);
	RUN_TEST(error_height);
	RUN_TEST(error_tilewidth);
	RUN_TEST(error_tileheight);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(basics);
	RUN_SUITE(errors);
	GREATEST_MAIN_END();

	return 0;
}

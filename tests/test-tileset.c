/*
 * test-tileset.c -- test tileset loader
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
#include <core/window.h>

#include <rpg/tileset-file.h>
#include <rpg/tileset.h>

GREATEST_TEST
test_basics_sample(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset;

	GREATEST_ASSERT(tileset_file_open(&loader, &tileset, DIRECTORY "sample-tileset.tileset"));
	GREATEST_ASSERT_EQ(64U, tileset.sprite->cellw);
	GREATEST_ASSERT_EQ(32U, tileset.sprite->cellh);

	GREATEST_ASSERT_EQ(4, tileset.tiledefsz);

	GREATEST_ASSERT_EQ(129, tileset.tiledefs[0].id);
	GREATEST_ASSERT_EQ(8, tileset.tiledefs[0].x);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[0].y);
	GREATEST_ASSERT_EQ(56, tileset.tiledefs[0].w);
	GREATEST_ASSERT_EQ(40, tileset.tiledefs[0].h);

	GREATEST_ASSERT_EQ(130, tileset.tiledefs[1].id);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[1].x);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[1].y);
	GREATEST_ASSERT_EQ(62, tileset.tiledefs[1].w);
	GREATEST_ASSERT_EQ(40, tileset.tiledefs[1].h);

	GREATEST_ASSERT_EQ(132, tileset.tiledefs[2].id);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[2].x);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[2].y);
	GREATEST_ASSERT_EQ(64, tileset.tiledefs[2].w);
	GREATEST_ASSERT_EQ(40, tileset.tiledefs[2].h);

	GREATEST_ASSERT_EQ(133, tileset.tiledefs[3].id);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[3].x);
	GREATEST_ASSERT_EQ(0, tileset.tiledefs[3].y);
	GREATEST_ASSERT_EQ(58, tileset.tiledefs[3].w);
	GREATEST_ASSERT_EQ(40, tileset.tiledefs[3].h);

	tileset_file_finish(&loader);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(test_basics_sample);
}

GREATEST_TEST
test_error_tilewidth(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	GREATEST_ASSERT(!tileset_file_open(&loader, &tileset, DIRECTORY "error-tilewidth.tileset"));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_tileheight(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	GREATEST_ASSERT(!tileset_file_open(&loader, &tileset, DIRECTORY "error-tileheight.tileset"));
	GREATEST_PASS();
}

GREATEST_TEST
test_error_image(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	GREATEST_ASSERT(!tileset_file_open(&loader, &tileset, DIRECTORY "error-image.tileset"));
	GREATEST_PASS();
}

GREATEST_SUITE(suite_errors)
{
	GREATEST_RUN_TEST(test_error_tilewidth);
	GREATEST_RUN_TEST(test_error_tileheight);
	GREATEST_RUN_TEST(test_error_image);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();

	if (core_init() && window_open("test-tileset", 100, 100)) {
		GREATEST_RUN_SUITE(suite_basics);
		GREATEST_RUN_SUITE(suite_errors);
	}

	GREATEST_MAIN_END();

	return 0;
}

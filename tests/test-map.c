/*
 * test-map.c -- test map routines
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

#include <rexo.h>

#include <core/core.h>
#include <core/error.h>
#include <core/panic.h>
#include <core/sys.h>
#include <core/window.h>

#include <rpg/map-file.h>
#include <rpg/map.h>

RX_TEST_CASE(test, basics_sample)
{
	struct map_file loader = {0};
	struct map map = {0};

	RX_INT_REQUIRE_EQUAL(map_file_open(&loader, &map, DIRECTORY "/maps/sample-map.map"), 0);
	RX_STR_REQUIRE_EQUAL(map.title, "This is a test map");
	RX_UINT_REQUIRE_EQUAL(map.columns, 4U);
	RX_UINT_REQUIRE_EQUAL(map.rows, 2U);

	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[0], 0U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[1], 1U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[2], 2U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[3], 3U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[4], 4U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[5], 5U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[6], 6U);
	RX_UINT_REQUIRE_EQUAL(map.layers[0].tiles[7], 7U);

	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[0], 8U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[1], 9U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[2], 10U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[3], 11U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[4], 12U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[5], 13U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[6], 14U);
	RX_UINT_REQUIRE_EQUAL(map.layers[1].tiles[7], 15U);

	RX_UINT_REQUIRE_EQUAL(map.tileset->sprite->cellw, 64U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->sprite->cellh, 32U);

	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefsz, 4);

	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[0].id, 129U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[0].x, 8U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[0].y, 0U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[0].w, 56U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[0].h, 40U);

	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[1].id, 130);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[1].x, 0);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[1].y, 0);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[1].w, 62);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[1].h, 40);

	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[2].id, 132U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[2].x, 0U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[2].y, 0U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[2].w, 64U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[2].h, 40U);

	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[3].id, 133U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[3].x, 0U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[3].y, 0U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[3].w, 58U);
	RX_UINT_REQUIRE_EQUAL(map.tileset->tiledefs[3].h, 40U);

	map_finish(&map);
	map_file_finish(&loader);
}

RX_TEST_CASE(test, error_title)
{
	struct map_file loader = {0};
	struct map map = {0};

	RX_INT_REQUIRE_EQUAL(map_file_open(&loader, &map, DIRECTORY "error-title.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

RX_TEST_CASE(test, error_columns)
{
	struct map_file loader = {0};
	struct map map = {0};

	RX_INT_REQUIRE_EQUAL(map_file_open(&loader, &map, DIRECTORY "error-columns.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

RX_TEST_CASE(test, error_rows)
{
	struct map_file loader = {0};
	struct map map = {0};

	RX_INT_REQUIRE_EQUAL(map_file_open(&loader, &map, DIRECTORY "error-rows.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

int
main(int argc, char **argv)
{
	/*
	 * This test opens graphical images and therefore need to initialize a
	 * window and all of the API. As tests sometime run on headless machine
	 * we will skip if it fails to initialize.
	 */

	if (core_init("fr.malikania", "test") < 0 || window_open("test-map", 100, 100) < 0)
		return 1;

	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

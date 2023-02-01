/*
 * test-map.c -- test map routines
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#include <mlk/core/core.h>
#include <mlk/core/error.h>
#include <mlk/core/panic.h>
#include <mlk/core/sys.h>
#include <mlk/core/window.h>

#include <mlk/rpg/map-file.h>
#include <mlk/rpg/map.h>

#include <dt.h>

static void
test_basics_sample(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	DT_EQ_INT(map_file_open(&loader, &map, DIRECTORY "/maps/sample-map.map"), 0);
	DT_EQ_STR(map.title, "This is a test map");
	DT_EQ_UINT(map.columns, 4U);
	DT_EQ_UINT(map.rows, 2U);

	DT_EQ_UINT(map.layers[0].tiles[0], 0U);
	DT_EQ_UINT(map.layers[0].tiles[1], 1U);
	DT_EQ_UINT(map.layers[0].tiles[2], 2U);
	DT_EQ_UINT(map.layers[0].tiles[3], 3U);
	DT_EQ_UINT(map.layers[0].tiles[4], 4U);
	DT_EQ_UINT(map.layers[0].tiles[5], 5U);
	DT_EQ_UINT(map.layers[0].tiles[6], 6U);
	DT_EQ_UINT(map.layers[0].tiles[7], 7U);

	DT_EQ_UINT(map.layers[1].tiles[0], 8U);
	DT_EQ_UINT(map.layers[1].tiles[1], 9U);
	DT_EQ_UINT(map.layers[1].tiles[2], 10U);
	DT_EQ_UINT(map.layers[1].tiles[3], 11U);
	DT_EQ_UINT(map.layers[1].tiles[4], 12U);
	DT_EQ_UINT(map.layers[1].tiles[5], 13U);
	DT_EQ_UINT(map.layers[1].tiles[6], 14U);
	DT_EQ_UINT(map.layers[1].tiles[7], 15U);

	DT_EQ_UINT(map.tileset->sprite->cellw, 64U);
	DT_EQ_UINT(map.tileset->sprite->cellh, 32U);

	DT_EQ_UINT(map.tileset->tiledefsz, 4);

	DT_EQ_UINT(map.tileset->tiledefs[0].id, 129U);
	DT_EQ_UINT(map.tileset->tiledefs[0].x, 8U);
	DT_EQ_UINT(map.tileset->tiledefs[0].y, 0U);
	DT_EQ_UINT(map.tileset->tiledefs[0].w, 56U);
	DT_EQ_UINT(map.tileset->tiledefs[0].h, 40U);

	DT_EQ_UINT(map.tileset->tiledefs[1].id, 130);
	DT_EQ_UINT(map.tileset->tiledefs[1].x, 0);
	DT_EQ_UINT(map.tileset->tiledefs[1].y, 0);
	DT_EQ_UINT(map.tileset->tiledefs[1].w, 62);
	DT_EQ_UINT(map.tileset->tiledefs[1].h, 40);

	DT_EQ_UINT(map.tileset->tiledefs[2].id, 132U);
	DT_EQ_UINT(map.tileset->tiledefs[2].x, 0U);
	DT_EQ_UINT(map.tileset->tiledefs[2].y, 0U);
	DT_EQ_UINT(map.tileset->tiledefs[2].w, 64U);
	DT_EQ_UINT(map.tileset->tiledefs[2].h, 40U);

	DT_EQ_UINT(map.tileset->tiledefs[3].id, 133U);
	DT_EQ_UINT(map.tileset->tiledefs[3].x, 0U);
	DT_EQ_UINT(map.tileset->tiledefs[3].y, 0U);
	DT_EQ_UINT(map.tileset->tiledefs[3].w, 58U);
	DT_EQ_UINT(map.tileset->tiledefs[3].h, 40U);

	map_finish(&map);
	map_file_finish(&loader);
}

static void
test_error_title(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	DT_EQ_INT(map_file_open(&loader, &map, DIRECTORY "error-title.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

static void
test_error_columns(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	DT_EQ_INT(map_file_open(&loader, &map, DIRECTORY "error-columns.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

static void
test_error_rows(void)
{
	struct map_file loader = {0};
	struct map map = {0};

	DT_EQ_INT(map_file_open(&loader, &map, DIRECTORY "error-rows.map"), -1);

	map_finish(&map);
	map_file_finish(&loader);
}

int
main(void)
{
	/*
	 * This test opens graphical images and therefore need to initialize a
	 * window and all of the API. As tests sometime run on headless machine
	 * we will skip if it fails to initialize.
	 */

	if (core_init("fr.malikania", "test") < 0 || window_open("test-map", 100, 100) < 0)
		return 1;

	DT_RUN(test_basics_sample);
	DT_RUN(test_error_title);
	DT_RUN(test_error_columns);
	DT_RUN(test_error_rows);
	DT_SUMMARY();

	return 0;
}

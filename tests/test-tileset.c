/*
 * test-tileset.c -- test tileset loader
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

#include <mlk/core/core.h>
#include <mlk/core/window.h>

#include <rpg/tileset-file.h>
#include <rpg/tileset.h>

#include <dt.h>

static void
test_basics_sample(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset;

	DT_EQ_INT(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/sample-tileset.tileset"), 0);
	DT_EQ_UINT(tileset.sprite->cellw, 64U);
	DT_EQ_UINT(tileset.sprite->cellh, 32U);

	DT_EQ_UINT(tileset.tiledefsz, 4U);

	DT_EQ_UINT(tileset.tiledefs[0].id, 129);
	DT_EQ_UINT(tileset.tiledefs[0].x, 8);
	DT_EQ_UINT(tileset.tiledefs[0].y, 0);
	DT_EQ_UINT(tileset.tiledefs[0].w, 56);
	DT_EQ_UINT(tileset.tiledefs[0].h, 40);

	DT_EQ_UINT(tileset.tiledefs[1].id, 130);
	DT_EQ_UINT(tileset.tiledefs[1].x, 0);
	DT_EQ_UINT(tileset.tiledefs[1].y, 0);
	DT_EQ_UINT(tileset.tiledefs[1].w, 62);
	DT_EQ_UINT(tileset.tiledefs[1].h, 40);

	DT_EQ_UINT(tileset.tiledefs[2].id, 132);
	DT_EQ_UINT(tileset.tiledefs[2].x, 0);
	DT_EQ_UINT(tileset.tiledefs[2].y, 0);
	DT_EQ_UINT(tileset.tiledefs[2].w, 64);
	DT_EQ_UINT(tileset.tiledefs[2].h, 40);

	DT_EQ_UINT(tileset.tiledefs[3].id, 133);
	DT_EQ_UINT(tileset.tiledefs[3].x, 0);
	DT_EQ_UINT(tileset.tiledefs[3].y, 0);
	DT_EQ_UINT(tileset.tiledefs[3].w, 58);
	DT_EQ_UINT(tileset.tiledefs[3].h, 40);

	tileset_file_finish(&loader);
}

static void
test_error_tilewidth(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	DT_EQ_INT(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-tilewidth.tileset"), -1);
}

static void
test_error_tileheight(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	DT_EQ_INT(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-tileheight.tileset"), -1);
}

static void
test_error_image(void)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	DT_EQ_INT(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-image.tileset"), -1);
}

int
main(void)
{
	if (core_init("fr.malikania", "test") < 0 || window_open("test-tileset", 100, 100) < 0)
		return 1;

	DT_RUN(test_basics_sample);
	DT_RUN(test_error_tilewidth);
	DT_RUN(test_error_tileheight);
	DT_RUN(test_error_image);
	DT_SUMMARY();
}

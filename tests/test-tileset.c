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

#include <core/core.h>
#include <core/window.h>

#include <rpg/tileset-file.h>
#include <rpg/tileset.h>

#include "test.h"

TEST_DECL(basics_sample)
{
	struct tileset_file loader = {0};
	struct tileset tileset;

	RX_REQUIRE(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/sample-tileset.tileset") == 0);
	RX_UINT_REQUIRE_EQUAL(tileset.sprite->cellw, 64U);
	RX_UINT_REQUIRE_EQUAL(tileset.sprite->cellh, 32U);

	RX_UINT_REQUIRE_EQUAL(tileset.tiledefsz, 4U);

	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[0].id, 129);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[0].x, 8);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[0].y, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[0].w, 56);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[0].h, 40);

	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[1].id, 130);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[1].x, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[1].y, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[1].w, 62);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[1].h, 40);

	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[2].id, 132);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[2].x, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[2].y, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[2].w, 64);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[2].h, 40);

	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[3].id, 133);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[3].x, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[3].y, 0);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[3].w, 58);
	RX_UINT_REQUIRE_EQUAL(tileset.tiledefs[3].h, 40);

	tileset_file_finish(&loader);
}

TEST_DECL(error_tilewidth)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	RX_INT_REQUIRE_EQUAL(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-tilewidth.tileset"), -1);
}

TEST_DECL(error_tileheight)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	RX_INT_REQUIRE_EQUAL(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-tileheight.tileset"), -1);
}

TEST_DECL(error_image)
{
	struct tileset_file loader = {0};
	struct tileset tileset = {0};

	RX_INT_REQUIRE_EQUAL(tileset_file_open(&loader, &tileset, DIRECTORY "/maps/error-image.tileset"), -1);
}

static const struct rx_test_case tests[] = {
	TEST_DEF("basics", "sample", basics_sample),
	TEST_DEF("error", "tilewidth", error_tilewidth),
	TEST_DEF("error", "tileheight", error_tileheight),
	TEST_DEF("error", "image", error_image),
};

int
main(int argc, char **argv)
{
	if (core_init("fr.malikania", "test") < 0 || window_open("test-tileset", 100, 100) < 0)
		return 1;

	return TEST_RUN(tests, argc, argv);
}

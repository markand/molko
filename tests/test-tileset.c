/*
 * test-tileset.c -- test tileset loader
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
#include <mlk/core/err.h>
#include <mlk/core/sprite.h>
#include <mlk/core/window.h>

#include <mlk/rpg/tileset-loader.h>
#include <mlk/rpg/tileset-loader-file.h>
#include <mlk/rpg/tileset.h>

#include <dt.h>

/*
 * Convenient struct that pack all the required data.
 */
struct tileset {
	struct mlk_tileset_loader_file loader;
	struct mlk_tileset tileset;
};

static inline int
tileset_open(struct tileset *ts, const char *path)
{
	mlk_tileset_loader_file_init(&ts->loader, path);

	return mlk_tileset_loader_open(&ts->loader.iface, &ts->tileset, path);
}

static inline void
tileset_finish(struct tileset *ts)
{
	mlk_tileset_loader_file_finish(&ts->loader);
}

static void
test_basics_sample(struct tileset *ts)
{
	DT_EQ_INT(tileset_open(ts, DIRECTORY "/maps/sample-tileset.tileset"), 0);
	DT_EQ_UINT(ts->tileset.sprite->cellw, 64U);
	DT_EQ_UINT(ts->tileset.sprite->cellh, 32U);

	DT_ASSERT(ts->tileset.collisions);
	DT_EQ_UINT(ts->tileset.collisionsz, 4U);

	DT_EQ_UINT(ts->tileset.collisions[0].id, 129);
	DT_EQ_UINT(ts->tileset.collisions[0].x, 8);
	DT_EQ_UINT(ts->tileset.collisions[0].y, 0);
	DT_EQ_UINT(ts->tileset.collisions[0].w, 56);
	DT_EQ_UINT(ts->tileset.collisions[0].h, 40);

	DT_EQ_UINT(ts->tileset.collisions[1].id, 130);
	DT_EQ_UINT(ts->tileset.collisions[1].x, 0);
	DT_EQ_UINT(ts->tileset.collisions[1].y, 0);
	DT_EQ_UINT(ts->tileset.collisions[1].w, 62);
	DT_EQ_UINT(ts->tileset.collisions[1].h, 40);

	DT_EQ_UINT(ts->tileset.collisions[2].id, 132);
	DT_EQ_UINT(ts->tileset.collisions[2].x, 0);
	DT_EQ_UINT(ts->tileset.collisions[2].y, 0);
	DT_EQ_UINT(ts->tileset.collisions[2].w, 64);
	DT_EQ_UINT(ts->tileset.collisions[2].h, 40);

	DT_EQ_UINT(ts->tileset.collisions[3].id, 133);
	DT_EQ_UINT(ts->tileset.collisions[3].x, 0);
	DT_EQ_UINT(ts->tileset.collisions[3].y, 0);
	DT_EQ_UINT(ts->tileset.collisions[3].w, 58);
	DT_EQ_UINT(ts->tileset.collisions[3].h, 40);
}

static void
test_error_tilewidth(struct tileset *ts)
{
	DT_EQ_INT(tileset_open(ts, DIRECTORY "/maps/error-tilewidth.tileset"), -1);
	DT_EQ_STR(mlk_err(), "missing tile dimensions before image");
}

static void
test_error_tileheight(struct tileset *ts)
{
	DT_EQ_INT(tileset_open(ts, DIRECTORY "/maps/error-tileheight.tileset"), -1);
	DT_EQ_STR(mlk_err(), "missing tile dimensions before image");
}

static void
test_error_image(struct tileset *ts)
{
	DT_EQ_INT(tileset_open(ts, DIRECTORY "/maps/error-image.tileset"), -1);
	DT_EQ_STR(mlk_err(), "missing tileset image");
}

static void
setup(struct tileset *ts)
{
	(void)ts;
}

static void
teardown(struct tileset *ts)
{
	tileset_finish(ts);
}

int
main(void)
{
	struct tileset ts;

	if (mlk_core_init("fr.malikania", "test") < 0 || mlk_window_open("test-tileset", 100, 100) < 0)
		return 1;

	DT_RUN_EX(test_basics_sample, setup, teardown, &ts);
	DT_RUN_EX(test_error_tilewidth, setup, teardown, &ts);
	DT_RUN_EX(test_error_tileheight, setup, teardown, &ts);
	DT_RUN_EX(test_error_image, setup, teardown, &ts);
	DT_SUMMARY();

	mlk_window_finish();
	mlk_core_finish();
}

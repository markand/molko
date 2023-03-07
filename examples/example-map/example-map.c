/*
 * example-map.c -- example on how to use a map
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <mlk/core/animation.h>
#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/image.h>
#include <mlk/core/key.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/rpg/map-loader-file.h>
#include <mlk/rpg/map-loader.h>
#include <mlk/rpg/map.h>
#include <mlk/rpg/tileset-loader-file.h>
#include <mlk/rpg/tileset-loader.h>
#include <mlk/rpg/tileset.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

#include <assets/maps/world.h>
#include <assets/tilesets/world.h>

static struct mlk_tileset_loader_file tileset_loader_file;
static struct mlk_tileset_loader tileset_loader;
static struct mlk_tileset tileset;

static struct mlk_map_loader_file map_loader_file;
static struct mlk_map_loader map_loader;
static struct mlk_map map;

static const struct {
	const char *basename;
	struct mlk_texture *texture;
} table_textures[] = {
	{ "world.png",                  &mlk_registry_textures[MLK_REGISTRY_TEXTURE_WORLD] },
	{ "animation-water.png",        &mlk_registry_textures[MLK_REGISTRY_TEXTURE_WATER] },
	{ NULL,                         NULL                                               }
};

static struct mlk_texture *
init_texture(struct mlk_tileset_loader *loader, const char *ident)
{
	(void)loader;

	char filepath[MLK_PATH_MAX], filename[FILENAME_MAX + 1];

	mlk_util_strlcpy(filepath, ident, sizeof (filepath));
	mlk_util_strlcpy(filename, mlk_util_basename(filepath), sizeof (filename));
	mlk_tracef("Searching for texture %s", filename);

	for (size_t i = 0; table_textures[i].basename != NULL; ++i)
		if (strcmp(table_textures[i].basename, filename) == 0)
			return table_textures[i].texture;

	return NULL;
}

struct mlk_tileset *
init_tileset(struct mlk_map_loader *loader, struct mlk_map *map, const char *ident)
{
	(void)loader;
	(void)map;
	(void)ident;

	/*
	 * For this example, we assume that the ident contains "world.tileset"
	 * because there are no other in the map we will load.
	 */
	mlk_tracef("Searching tileset %s", ident);

	if (mlk_tileset_loader_openmem(&tileset_loader, &tileset, assets_tilesets_world, sizeof (assets_tilesets_world)) < 0)
		mlk_panic();

	return &tileset;
}

static void
init(void)
{
	if (mlk_example_init("example-map") < 0)
		mlk_panic();

	/*
	 * Just like in example-tileset.c, we want to open images from the
	 * registry which is not supported by itself in
	 * mlk_tileset_loader_file.
	 */
	mlk_tileset_loader_file_init(&tileset_loader_file, &tileset_loader, "");
	tileset_loader.init_texture = init_texture;

	/*
	 * Create our map loader. It will also search for a tileset to be found
	 * on disk by default which we would like to avoid. We override the
	 * init_tileset function.
	 */
	mlk_map_loader_file_init(&map_loader_file, &map_loader, "");
	map_loader.init_tileset = init_tileset;

	if (mlk_map_loader_openmem(&map_loader, &map, assets_maps_world, sizeof (assets_maps_world)) < 0)
		mlk_panic();

	// TODO: this not handled by the map yet.
	map.player_sprite = &mlk_registry_sprites[MLK_REGISTRY_TEXTURE_JOHN];
	mlk_map_init(&map);
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		mlk_map_handle(&map, ev);
		break;
	}
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	(void)st;

	mlk_map_update(&map, ticks);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_map_draw(&map);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init();
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	mlk_map_loader_file_finish(&map_loader_file);
	mlk_tileset_loader_file_finish(&tileset_loader_file);
	mlk_example_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	quit();
}

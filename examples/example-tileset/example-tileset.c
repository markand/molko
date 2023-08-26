/*
 * example-tileset.c -- example on how to use a tileset
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

#include <mlk/util/util.h>

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

#include <mlk/rpg/tileset-loader-file.h>
#include <mlk/rpg/tileset-loader.h>
#include <mlk/rpg/tileset.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

#include <assets/tilesets/world.h>

static struct mlk_tileset_loader_file loader;
static struct mlk_tileset tileset;

/*
 * This is only to demonstrate how we can custom-allocate data, you can skip
 * that part.
 */
static struct mlk_sprite sprites[16];
static size_t spritesz;
static struct mlk_animation animations[16];
static size_t animationsz;

static const struct {
	const char *basename;
	struct mlk_texture *texture;
} table_textures[] = {
	{ "world.png",                  &mlk_registry_textures[MLK_REGISTRY_TEXTURE_WORLD] },
	{ "animation-water.png",        &mlk_registry_textures[MLK_REGISTRY_TEXTURE_WATER] },
	{ NULL,                         NULL                                               }
};

static struct mlk_texture *
new_texture(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset, const char *ident)
{
	(void)loader;
	(void)tileset;

	char filepath[MLK_PATH_MAX], filename[FILENAME_MAX + 1];

	mlk_util_strlcpy(filepath, ident, sizeof (filepath));
	mlk_util_strlcpy(filename, mlk_util_basename(filepath), sizeof (filename));
	mlk_tracef("Searching for texture %s", filename);

	for (size_t i = 0; table_textures[i].basename != NULL; ++i)
		if (strcmp(table_textures[i].basename, filename) == 0)
			return table_textures[i].texture;

	return NULL;
}

static struct mlk_sprite *
new_sprite(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset)
{
	(void)loader;
	(void)tileset;

	/* Just ensure we haven't reach the limit. */
	assert(spritesz < MLK_UTIL_SIZE(sprites));

	return &sprites[spritesz++];
}

static struct mlk_animation *
new_animation(struct mlk_tileset_loader *loader, struct mlk_tileset *tileset)
{
	(void)loader;
	(void)tileset;

	/* Just ensure we haven't reach the limit. */
	assert(animationsz < MLK_UTIL_SIZE(animations));

	return &animations[animationsz++];
}

static void
init(void)
{
	if (mlk_example_init("example-tileset") < 0)
		mlk_panic();

	/*
	 * Demonstrate how we can override functions to use different resources
	 * and/or allocations.
	 *
	 * Images are loaded from the libmlk-example registry from RAM and
	 * sprites animations are statically allocated.
	 */
	mlk_tileset_loader_file_init(&loader, "");
	loader.iface.new_texture = new_texture;
	loader.iface.new_sprite = new_sprite;
	loader.iface.new_animation = new_animation;

	if (mlk_tileset_loader_openmem(&loader.iface, &tileset, assets_tilesets_world, sizeof (assets_tilesets_world)) < 0)
		mlk_panic();
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
		break;
	}
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	(void)st;

	mlk_tileset_update(&tileset, ticks);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	int nc, nr;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();

	/*
	 * Draw the animated tile all over the screen.
	 */
	nc = (MLK_EXAMPLE_W / tileset.sprite->cellw) + 1;
	nr = (MLK_EXAMPLE_H / tileset.sprite->cellh) + 1;

	for (int r = 0; r < nr; ++r) {
		for (int c = 0; c < nc; ++c) {
			mlk_tileset_draw(&tileset, 7, 22,
			    c * tileset.sprite->cellw,
			    r * tileset.sprite->cellh);
		}
	}

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
	mlk_tileset_loader_file_finish(&loader);
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

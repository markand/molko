/*
 * example-map.c -- show how to use maps
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/image.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/texture.h>
#include <core/window.h>

#include <ui/ui.h>

#include <rpg/map.h>
#include <rpg/map-file.h>
#include <rpg/rpg.h>
#include <rpg/teleport.h>

#include <assets/sprites/john.h>

#define W       (1280)
#define H       (720)

static struct action *load_action(struct map *, int, int, int, int, const char *);

struct map_state {
	struct map_file loader;
	struct map map;
	struct state state;
};

static struct texture john_texture;
static struct sprite john_sprite;

static void
map_state_start(struct state *st)
{
	struct map_state *ms = st->data;

	game.inhibit &= ~(INHIBIT_STATE_INPUT);

	if (!map_init(&ms->map))
		panic();
}

static void
map_state_update(struct state *st, unsigned int ticks)
{
	struct map_state *ms = st->data;

	map_update(&ms->map, ticks);
}

static void
map_state_handle(struct state *st, const union event *ev)
{
	struct map_state *ms = st->data;

	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		map_handle(&ms->map, ev);
		break;
	}
}

static void
map_state_draw(struct state *st)
{
	struct map_state *ms = st->data;

	painter_set_color(0xffffffff);
	painter_clear();
	map_draw(&ms->map);
	painter_present();
}

static void
map_state_finish(struct state *state)
{
	struct map_state *ms = state->data;

	map_finish(&ms->map);
	map_file_finish(&ms->loader);
}

static struct state *
map_state_new(const char *name)
{
	char path[1024];
	struct map_state *ms;

	ms = alloc_zero(1, sizeof (*ms));
	ms->loader.load_action = load_action;

	snprintf(path, sizeof (path), "%s/assets/maps/%s.map", BINDIR, name);

	if (!map_file_open(&ms->loader, &ms->map, path))
		panic();

	/* TODO: we may need to add a function in loader. */
	ms->map.player_sprite = &john_sprite;

	map_init(&ms->map);
	ms->state.data = ms;
	ms->state.start = map_state_start;
	ms->state.handle = map_state_handle;
	ms->state.update = map_state_update;
	ms->state.draw = map_state_draw;
	ms->state.finish = map_state_finish;

	return &ms->state;
}

struct teleport_effect {
	struct action act;
	struct teleport tp;
};

static void
teleport_effect_draw(struct action *act)
{
	struct teleport_effect *fx = act->data;

	teleport_draw(&fx->tp);
}

static bool
teleport_effect_update(struct action *act, unsigned int ticks)
{
	struct teleport_effect *fx = act->data;

	return teleport_update(&fx->tp, ticks);
}

static void
teleport_effect_finish(struct action *act)
{
	struct teleport_effect *fx = act->data;

	teleport_finish(&fx->tp);
}

static void
teleport_effect(struct map *current_map, const char *name)
{
	struct teleport_effect *fx;

	fx = alloc_zero(1, sizeof (*fx));
	fx->tp.state = map_state_new(name);

	fx->act.data = fx;
	fx->act.draw = teleport_effect_draw;
	fx->act.update = teleport_effect_update;
	fx->act.finish = teleport_effect_finish;

	/* Stop movement. */
	current_map->player_movement = 0;

	/* Player isn't allowed to move anymore. */
	game.inhibit = INHIBIT_STATE_INPUT;

	teleport_start(&fx->tp);
	action_stack_add(&current_map->actions, &fx->act);
}

struct teleport_touch {
	struct action act;
	struct map *map;        /* Current map. */
	char name[256];
	int x;
	int y;
	unsigned int w;
	unsigned int h;
};

static bool
teleport_touch_update(struct action *act, unsigned int ticks)
{
	(void)ticks;

	const struct teleport_touch *touch = act->data;

	const int x = touch->x - touch->map->player_sprite->cellw;
	const int y = touch->y - touch->map->player_sprite->cellh;
	const unsigned int w = touch->w + touch->map->player_sprite->cellw;
	const unsigned int h = touch->h + touch->map->player_sprite->cellh;

	if (maths_is_boxed(x, y, w, h, touch->map->player_x, touch->map->player_y)) {
		teleport_effect(touch->map, touch->name);
		return true;
	}

	return false;
}

static void
teleport_touch_finish(struct action *act)
{
	free(act->data);
}

static struct action *
teleport_touch_new(struct map *map, int x, int y, int w, int h, const char *name)
{
	struct teleport_touch *touch;

	touch = alloc_zero(1, sizeof (*touch));
	touch->map = map;
	touch->x = x;
	touch->y = y;
	touch->w = w;
	touch->h = h;
	snprintf(touch->name, sizeof (touch->name), "%s", name);

	touch->act.data = touch;
	touch->act.update = teleport_touch_update;
	touch->act.finish = teleport_touch_finish;

	return &touch->act;
}

static struct action *
load_action(struct map *map, int x, int y, int w, int h, const char *id)
{
	if (strncmp(id, "teleport:", 9) == 0)
		return teleport_touch_new(map, x, y, w, h, id + 9);

	return NULL;
}

static void
init(void)
{
	if (!core_init() || !ui_init())
		panic();
	if (!window_open("Example - Map", W, H))
		panic();
	if (!image_openmem(&john_texture, sprites_john, sizeof (sprites_john)))
		panic();

	sprite_init(&john_sprite, &john_texture, 48, 48);
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	core_finish();
}

static void
run(void)
{
	game_switch(map_state_new("map-world"), true);
	game_loop();
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

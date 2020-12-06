/*
 * map.c -- map state
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

#include <compat.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>

#include <rpg/map.h>
#include <rpg/map-file.h>

#include <adventure/actions/spawner.h>
#include <adventure/actions/teleport.h>

#include "molko.h"
#include "map.h"

struct self {
	struct state state;
	char name[FILENAME_MAX];
	int origin_x;
	int origin_y;
	struct map map;
	struct map_file map_file;
};

static void
load_spawner(struct map *map, int x, int y, int w, int h, const char *value)
{
	(void)x;
	(void)y;
	(void)w;
	(void)h;
	(void)value;

	action_stack_add(&map->astack_par, spawner_new(map, 100, 300));
}

static void
load_teleport(struct map *map, int x, int y, int w, int h, const char *value)
{
	char name[128] = {0};
	int origin_x = -1, origin_y = -1;

	sscanf(value, "%127[^|]|%d|%d", name, &origin_x, &origin_y);
	action_stack_add(&map->astack_par, teleport_new(map, name, x, y, w, h, origin_x, origin_y));
}

static void
load_action(struct map *map, int x, int y, int w, int h, const char *value)
{
	static const struct {
		const char *name;
		void (*load)(struct map *, int, int, int, int, const char *);
	} table[] = {
		{ "teleport|",  load_teleport },
		{ "spawner|",   load_spawner }
	};

	for (size_t i = 0; i < NELEM(table); ++i) {
		size_t len = strlen(table[i].name);

		if (strncmp(table[i].name, value, len) == 0) {
			table[i].load(map, x, y, w, h, value + len);
			break;
		}
	}
}

static void
start(struct state *state)
{
	struct self *self = state->data;

	self->map_file.load_action = load_action;

	if (!map_file_open(&self->map_file, &self->map, molko_path(self->name)))
		panic();

	/* TODO: find this from team maybe. */
	if (!image_open(&molko.map_player_texture, molko_path("assets/sprites/john.png")))
		panic();

	sprite_init(&molko.map_player_sprite, &molko.map_player_texture, 48, 48);
	self->map.player_sprite = &molko.map_player_sprite;

	/* TODO: add support for saving origin in save. */
	if (self->origin_x >= 0)
		self->map.player_x = self->origin_x;
	if (self->origin_y >= 0)
		self->map.player_y = self->origin_y;

	if (!map_init(&self->map))
		panic();
}

static void
handle(struct state *state, const union event *ev)
{
	struct self *self = state->data;

	map_handle(&self->map, ev);
}

static void
update(struct state *state, unsigned int ticks)
{
	struct self *self = state->data;

	map_update(&self->map, ticks);
}

static void
draw(struct state *state)
{
	struct self *self = state->data;

	painter_clear();
	map_draw(&self->map);
	painter_present();
}

static void
finish(struct state *state)
{
	struct self *self = state->data;

	map_finish(&self->map);
	map_file_finish(&self->map_file);

	free(self);
}

struct state *
map_state_new(const char *name, int origin_x, int origin_y)
{
	(void)origin_x;
	(void)origin_y;

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->origin_x = origin_x;
	self->origin_y = origin_y;
	strlcpy(self->name, name, sizeof (self->name));

	self->state.data = self;
	self->state.start = start;
	self->state.handle = handle;
	self->state.update = update;
	self->state.draw = draw;
	self->state.finish = finish;

	return &self->state;
}

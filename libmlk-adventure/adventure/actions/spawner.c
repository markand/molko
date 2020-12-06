/*
 * spawner.c -- spawn battle while moving
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

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <core/alloc.h>
#include <core/game.h>
#include <core/util.h>

#include <rpg/map.h>

#include "spawner.h"

struct self {
	struct action action;
	struct map *map;
	int last_x;
	int last_y;
	unsigned int steps;
};

static inline unsigned int
distance(const struct self *self)
{
	unsigned int gap_x = fmax(self->last_x, self->map->player_x) -
	                     fmin(self->last_x, self->map->player_x);
	unsigned int gap_y = fmax(self->last_y, self->map->player_y) -
			     fmin(self->last_y, self->map->player_y);

	return fmin(self->steps, gap_x + gap_y);
}

static bool
update(struct action *act, unsigned int ticks)
{
	(void)ticks;

	struct self *self = act->data;

	if (self->map->player_movement) {
		self->steps -= distance(self);
		self->last_x = self->map->player_x;
		self->last_y = self->map->player_y;

		if (self->steps == 0) {
			/* TODO: start battle here. */
			return false;
		}
	}

	return false;
}

static void
finish(struct action *act)
{
	free(act->data);
}

struct action *
spawner_new(struct map *map, unsigned int low, unsigned int high)
{
	assert(map);

	struct self *self;

	self = alloc_new0(sizeof (*self));
	self->map = map;
	self->last_x = map->player_x;
	self->last_y = map->player_y;
	self->steps = nrand(low, high);

	self->action.data = self;
	self->action.update = update;
	self->action.finish = finish;

	return &self->action;
}

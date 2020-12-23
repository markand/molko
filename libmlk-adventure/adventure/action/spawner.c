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

#include <rpg/battle.h>
#include <rpg/map.h>

#include <adventure/molko.h>

#include <adventure/character/black-cat.h>

#include "spawner.h"

static inline unsigned int
distance(const struct spawner *s)
{
	unsigned int gap_x = fmax(s->last_x, s->map->player_x) -
	                     fmin(s->last_x, s->map->player_x);
	unsigned int gap_y = fmax(s->last_y, s->map->player_y) -
			     fmin(s->last_y, s->map->player_y);

	return fmin(s->steps, gap_x + gap_y);
}

static void
fight(void)
{
	/* TODO: */
	struct battle *bt;

	bt = alloc_new0(sizeof (*bt));
	bt->enemies[0].ch = &character_black_cat;
	bt->enemies[0].x = 400;
	bt->enemies[0].y = 50;
	bt->inventory = &molko.inventory;

	for (size_t i = 0; i < TEAM_MEMBER_MAX; ++i)
		bt->team[i].ch = molko.team.members[i];

	molko_fight(bt);
}

static bool
update(struct action *act, unsigned int ticks)
{
	(void)ticks;

	struct spawner *s = act->data;

	if (s->map->player_movement) {
		s->steps -= distance(s);
		s->last_x = s->map->player_x;
		s->last_y = s->map->player_y;

		if (s->steps == 0) {
			s->steps = util_nrand(s->low, s->high);
			fight();
		}
	}

	return false;
}

void
spawner_init(struct spawner *s)
{
	assert(s);

	s->last_x = s->map->player_x;
	s->last_y = s->map->player_y;
	s->steps = util_nrand(s->low, s->high);
}

struct action *
spawner_action(struct spawner *s)
{
	assert(s);

	s->action.data = s;
	s->action.update = update;

	spawner_init(s);

	return &s->action;
}

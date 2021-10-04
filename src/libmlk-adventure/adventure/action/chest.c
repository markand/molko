/*
 * chest.c -- chest object
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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
#include <stdlib.h>
#include <string.h>

#include <core/animation.h>
#include <core/event.h>
#include <core/maths.h>
#include <core/panic.h>
#include <core/sound.h>
#include <core/sprite.h>

#include <rpg/map.h>
#include <rpg/save.h>
#include <rpg/property.h>

#include "chest.h"

#define X(c)            ((c)->x - ((c)->map)->view_x)
#define Y(c)            ((c)->y - ((c)->map)->view_y)
#define W(c)            ((c)->animation.sprite->cellw)
#define H(c)            ((c)->animation.sprite->cellh)
#define TOLERANCE       (10)

static int
is_near(const struct chest *c)
{
	const int x          = c->x - c->map->player_sprite->cellw - TOLERANCE;
	const int y          = c->y - c->map->player_sprite->cellh - TOLERANCE;
	const unsigned int w = W(c) + c->map->player_sprite->cellw + (TOLERANCE * 2);
	const unsigned int h = H(c) + c->map->player_sprite->cellh + (TOLERANCE * 2);

	return maths_is_boxed(x, y, w, h, c->map->player_x, c->map->player_y);
}

static void
invoke(struct chest *c)
{
	c->state = CHEST_STATE_ANIMATE;

	animation_start(&c->animation);

	if (c->sound)
		sound_play(c->sound, -1, 0);
}

static void
handle(struct action *act, const union event *ev)
{
	struct chest *c = act->data;

	if (!is_near(c) || c->state != CHEST_STATE_CLOSED)
		return;

	switch (ev->type) {
	case EVENT_KEYDOWN:
		if (ev->key.key == KEY_ENTER)
			invoke(c);
		break;
	case EVENT_CLICKDOWN:
		if (maths_is_boxed(X(c), Y(c), W(c), H(c), ev->click.x, ev->click.y))
			invoke(c);
		break;
	default:
		break;
	}
}

static int
update(struct action *act, unsigned int ticks)
{
	struct chest *c = act->data;

	if (c->state != CHEST_STATE_ANIMATE)
		return 0;

	if (animation_update(&c->animation, ticks)) {
		c->state = CHEST_STATE_OPEN;

		if (c->exec)
			c->exec(c);
	}

	return 0;
}

static void
draw(struct action *act)
{
	struct chest *c = act->data;

	switch (c->state) {
	case CHEST_STATE_OPEN:
		sprite_draw(
		    c->animation.sprite,
		    c->animation.sprite->nrows - 1,
		    c->animation.sprite->ncols - 1,
		    X(c),
		    Y(c)
		);
		break;
	case CHEST_STATE_ANIMATE:
		animation_draw(&c->animation, X(c), Y(c));
		break;
	default:
		sprite_draw(c->animation.sprite, 0, 0, X(c), Y(c));
		break;
	}
}

void
chest_init(struct chest *c)
{
	assert(c);

	if (c->save && c->property) {
		if (property_load(c->property, c->save) < 0)
			panic();

		/* TODO: add an utility. */
		if (strcmp(c->property->value, "true") == 0)
			c->state = CHEST_STATE_OPEN;
	}
}

struct action *
chest_action(struct chest *c)
{
	assert(c);

	c->action.data = c;
	c->action.handle = handle;
	c->action.update = update;
	c->action.draw = draw;

	return &c->action;
}

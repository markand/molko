/*
 * chest.c -- animated chest
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

#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/sprite.h>

#include <mlk/example/registry.h>

#include "chest.h"

#define CHEST_DELAY 80

static void
handle(struct mlk_action *act, const union mlk_event *ev)
{
	struct chest *chest = act->data;
	unsigned int cw, ch;

	/* Make sure that we don't operate on a already opened chest. */
	if (chest->state == CHEST_STATE_OPENED)
		return;

	/*
	 * We are only interested if the event is a click on the chest itself
	 * so we have to test because actions have no notion of geometry.
	 */
	if (ev->type != MLK_EVENT_CLICKDOWN)
		return;

	cw = registry_sprites[REGISTRY_TEXTURE_CHEST].cellw;
	ch = registry_sprites[REGISTRY_TEXTURE_CHEST].cellh;

	if (!mlk_maths_is_boxed(chest->x, chest->y, cw, ch, ev->click.x, ev->click.y))
		return;

	mlk_animation_start(&chest->animation);
	chest->state = CHEST_STATE_OPENING;
}

static int
update(struct mlk_action *act, unsigned int ticks)
{
	struct chest *chest = act->data;

	if (chest->state != CHEST_STATE_OPENING)
		return 0;

	if (mlk_animation_update(&chest->animation, ticks)) {
		chest->state = CHEST_STATE_OPENED;

		if (chest->run)
			chest->run(chest);
	}

	/* The chest never dies. */
	return 0;
}

static void
draw(struct mlk_action *act)
{
	const struct chest *chest = act->data;

	switch (chest->state) {
	case CHEST_STATE_CLOSED:
		mlk_sprite_draw(chest->animation.sprite, 0, 0, chest->x, chest->y);
		break;
	case CHEST_STATE_OPENING:
		mlk_animation_draw(&chest->animation, chest->x, chest->y);
		break;
	case CHEST_STATE_OPENED:
		mlk_sprite_draw(chest->animation.sprite,
		    chest->animation.sprite->nrows - 1,
		    chest->animation.sprite->ncols - 1,
		    chest->x,
		    chest->y);
		break;
	default:
		break;
	}
}

struct mlk_action *
chest_init(struct chest *chest)
{
	assert(chest);

	chest->state = CHEST_STATE_CLOSED;
	chest->action.data = chest;
	chest->action.handle = handle;
	chest->action.update = update;
	chest->action.draw = draw;

	mlk_animation_init(&chest->animation, &registry_sprites[REGISTRY_TEXTURE_CHEST], CHEST_DELAY);

	return &chest->action;
}

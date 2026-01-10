/*
 * chest.c -- animated chest
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <mlk/core/coro.h>
#include <mlk/core/event.h>
#include <mlk/core/maths.h>
#include <mlk/core/sound.h>
#include <mlk/core/sprite.h>

#include <mlk/example/registry.h>

#include "chest.h"

#define CHEST_DELAY 80

static void
chest_handle(struct mlk_action *self, const union mlk_event *ev)
{
	struct chest *chest;
	unsigned int cw, ch;

	chest = CHEST(self, action);

	/* Make sure that we don't operate on a already opened chest. */
	if (chest->state != CHEST_STATE_CLOSED)
		return;

	/*
	 * We are only interested if the event is a click on the chest itself
	 * so we have to test because actions have no notion of geometry.
	 */
	if (ev->type != MLK_EVENT_CLICKDOWN)
		return;

	cw = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellw;
	ch = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellh;

	if (!mlk_maths_is_boxed(ev->click.x, ev->click.y, chest->x, chest->y, cw, ch))
		return;

	mlk_sound_play(&mlk_registry_sounds[MLK_REGISTRY_SOUND_OPEN_CHEST]);
	mlk_animation_start(&chest->animation);

	chest->state = CHEST_STATE_OPENING;
}

static int
chest_update(struct mlk_drawable *self, unsigned int ticks)
{
	struct chest *chest = CHEST(self, drawable);

	if (chest->state != CHEST_STATE_OPENING)
		return 0;

	if (mlk_animation_update(&chest->animation, ticks))
		chest->state = CHEST_STATE_OPENED;

	/* Chest handle dies but not its drawable. */
	return 0;
}

static void
chest_draw(struct mlk_drawable *self)
{
	const struct chest *chest = MLK_UTIL_CONTAINER_OF(self, struct chest, drawable);

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

void
chest_pressed(const struct chest *chest)
{
	assert(chest);

	while (chest->state == CHEST_STATE_CLOSED)
		mlk_coro_yield();
}

void
chest_opened(const struct chest *chest)
{
	assert(chest);

	while (chest->state != CHEST_STATE_OPENED)
		mlk_coro_yield();
}

void
chest_restart(struct chest *chest)
{
	assert(chest);

	chest->state = CHEST_STATE_CLOSED;

	chest->animation.sprite = &mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST];
	chest->animation.delay = CHEST_DELAY;

	chest->action.handle = chest_handle;

	chest->drawable.update = chest_update;
	chest->drawable.draw = chest_draw;
}

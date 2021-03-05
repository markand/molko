/*
 * potion.h -- give some heal points
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

#include <math.h>

#include <core/sound.h>

#include <rpg/battle.h>
#include <rpg/character.h>
#include <rpg/item.h>

#include <adventure/adventure_p.h>
#include <adventure/assets.h>

#include "potion.h"

static void
heal(struct character *ch)
{
	ch->hp = fmin(ch->hp + 50, ch->hpmax);
	sound_play(&assets_sounds[ASSETS_SOUND_ITEM_POTION], -1, 0);
}

static void
exec_menu(const struct item *item, struct character *ch)
{
	(void)item;

	heal(ch);
}

static void
exec_battle(const struct item *item, struct battle *bt, struct character *src, struct character *tgt)
{
	(void)item;

	heal(tgt);
	battle_indicator_hp(bt, tgt, 50);
}

const struct item item_potion = {
	.name = N_("Potion"),
	.description = N_("Recover 50 HP."),
	.exec_menu = exec_menu,
	.exec_battle = exec_battle
};
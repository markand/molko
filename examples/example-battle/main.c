/*
 * example-battle.c -- show how to use battle
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sprite.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>
#include <ui/label.h>
#include <ui/theme.h>
#include <ui/ui.h>

#include <rpg/character.h>
#include <rpg/battle.h>
#include <rpg/rpg.h>
#include <rpg/spell.h>

#include "registry.h"
#include "spell-fire.h"

#define W 1280
#define H 720

static void
adventurer_reset(struct character *ch)
{
	/* TODO: this function should compute the attack thanks to the level. */
	ch->hpmax = 120;
	ch->mpmax = 50;
	ch->atk = 50;
	ch->def = 50;
	ch->agt = 50;
	ch->luck = 50;
}

static void
haunted_wood_reset(struct character *ch)
{
	ch->hpmax = ch->hp = 2000;
	ch->mpmax = ch->mp = 250;
	ch->atk = 178;
	ch->def = 80;
	ch->agt = 80;
	ch->luck = 100;
}

static void
black_cat_reset(struct character *ch)
{
	ch->hpmax = ch->hp = 126;
	ch->mpmax = ch->mp = 38;
	ch->atk = 22;
	ch->def = 19;
	ch->agt = 11;
	ch->luck = 14;
}

static struct character team[] = {
	{
		.name = "Molko",
		.type = "Adventurer",
		.level = 1,
		.hp = 120,
		.mp = 50,
		.reset = adventurer_reset,
		.sprites = {
			[CHARACTER_SPRITE_NORMAL] = &registry_sprites[REGISTRY_TEXTURE_JOHN_WALK],
			[CHARACTER_SPRITE_SWORD] = &registry_sprites[REGISTRY_TEXTURE_JOHN_SWORD],
		},
		.spells = {
			&spell_fire
		}
	},
};

static void
haunted_wood_strat(struct character *ch, struct battle *bt)
{
	(void)ch;

	/* TODO: Select randomly. */
	battle_attack(bt, bt->order_cur->ch, bt->team[0].ch);
}

static void
black_cat_strat(struct character *ch, struct battle *bt)
{
	(void)ch;

	/* TODO: Select randomly. */
	battle_attack(bt, bt->order_cur->ch, bt->team[0].ch);
}

static struct character haunted_wood = {
	.name = "Haunted Wood",
	.type = "Wood",
	.level = 30,
	.reset = haunted_wood_reset,
	.sprites = {
		[CHARACTER_SPRITE_NORMAL] = &registry_sprites[REGISTRY_TEXTURE_HAUNTED_WOOD],
	},
	.exec = haunted_wood_strat
};

static struct character black_cat = {
	.name = "Black Cat",
	.type = "Cat",
	.level = 6,
	.reset = black_cat_reset,
	.sprites = {
		[CHARACTER_SPRITE_NORMAL] = &registry_sprites[REGISTRY_TEXTURE_BLACK_CAT],
	},
	.exec = black_cat_strat
};

static void
init(void)
{
	if (!core_init("fr.malikania", "battle") || !ui_init() || !rpg_init())
		panic();
	if (!window_open("Example - Battle", W, H))
		panic();

	registry_init();

	/* Set cursor in default theme. */
	theme_default()->sprites[THEME_SPRITE_CURSOR] = &registry_sprites[REGISTRY_TEXTURE_CURSOR];
}

static struct state fight_state;

static void
prepare_to_fight(void)
{
	struct battle *bt = alloc_new0(sizeof (*bt));

//	bt->enemies[0].ch = &haunted_wood;
	bt->team[0].ch = &team[0];
	//bt->team[1].ch = &team[1];

	/* Positionate the single ennemy to the left. */
	align(ALIGN_LEFT,
	    &bt->enemies[0].x, &bt->enemies[0].y,
	    haunted_wood.sprites[CHARACTER_SPRITE_NORMAL]->cellw,
	    haunted_wood.sprites[CHARACTER_SPRITE_NORMAL]->cellh,
	    0, 0, window.w, window.h);

	/* Black cat is near the previous monster. */
	bt->enemies[1].ch = &black_cat;
	bt->enemies[1].x = 500;
	bt->enemies[1].y = 100;

	bt->background = &registry_images[REGISTRY_IMAGE_BATTLE_BACKGROUND];

	battle_start(bt);

	fight_state.data = bt;
	game_switch(&fight_state, false);
}


static void
empty_handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	case EVENT_KEYDOWN:
		if (ev->key.key == KEY_SPACE)
			prepare_to_fight();
		break;
	default:
		break;
	}
}

static void
empty_draw(struct state *st)
{
	(void)st;

	static const struct label info = {
		.text = "Press <Space> to start a battle.",
		.x = 10,
		.y = 10,
		.flags = LABEL_FLAGS_SHADOW
	};

	painter_set_color(0x4f8fbaff);
	painter_clear();
	label_draw(&info);
	painter_present();
}

static struct state empty_state = {
	.handle = empty_handle,
	.draw = empty_draw
};

static void
fight_handle(struct state *st, const union event *ev)
{
	battle_handle(st->data, ev);
}

static void
fight_update(struct state *st, unsigned int ticks)
{
	struct battle *bt = st->data;

	if (battle_update(bt, ticks))
		game_switch(&empty_state, false);
}

static void
fight_draw(struct state *st)
{
	painter_set_color(0x000000ff);
	painter_clear();
	battle_draw(st->data);
	painter_present();
}

static void
fight_finish(struct state *st)
{
	battle_finish(st->data);
	free(st->data);
}

static struct state fight_state = {
	.handle = fight_handle,
	.update = fight_update,
	.draw = fight_draw,
	.finish = fight_finish,
};

static void
run(void)
{
	game_switch(&empty_state, true);
	game_loop();
}

static void
quit(void)
{
	registry_finish();
	theme_finish();
	window_finish();
	sys_finish();
}

int
main(int argc, char **argv)
{
	--argc;
	++argv;

	init();
	run();
	quit();

	return 0;
}

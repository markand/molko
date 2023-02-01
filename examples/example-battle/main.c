/*
 * example-battle.c -- show how to use battle
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

#include <rpg/battle-bar-default.h>
#include <rpg/battle-bar.h>
#include <rpg/battle.h>
#include <rpg/character.h>
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

#if 0

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

#endif

static void
black_cat_reset(struct character *ch)
{
	ch->hpmax = ch->hp = 120;
	ch->mpmax = ch->mp = 38;
	ch->atk = 22;
	ch->def = 19;
	ch->agt = 11;
	ch->luck = 14;
}

#if 0

static void
haunted_wood_strat(struct character *ch, struct battle *bt)
{
	(void)ch;

	/* TODO: Select randomly. */
	battle_attack(bt, battle_current(bt)->ch, bt->team[0]->ch);
}

#endif

static void
black_cat_strat(struct character *ch, struct battle *bt)
{
	(void)ch;

	/* TODO: Select randomly. */
	battle_attack(bt, battle_current(bt)->ch, bt->team[0]->ch);
}

#if 0

static struct character haunted_wood = {
	.name = "Haunted Wood",
	.level = 30,
	.reset = haunted_wood_reset,
	.sprites = {
		[CHARACTER_SPRITE_NORMAL] = &registry_sprites[REGISTRY_TEXTURE_HAUNTED_WOOD],
	},
	.exec = haunted_wood_strat
};

#endif

static void
init(void)
{
	if (core_init("fr.malikania", "example-battle") < 0 || ui_init() < 0 || rpg_init() < 0)
		panic();
	if (window_open("Example - Battle", W, H) < 0)
		panic();

	registry_init();

	/* Set cursor in default theme. */
	theme_default()->sprites[THEME_SPRITE_CURSOR] = &registry_sprites[REGISTRY_TEXTURE_CURSOR];
}

static struct state *states[2];
static struct state fight_state;

static struct {
	struct character ch;
	struct battle_entity entity;
} entities[] = {
	/* == Enemies == */
	{
		.ch = {
			.name = "Black Cat",
			.level = 6,
			.reset = black_cat_reset,
			.sprites = {
				[CHARACTER_SPRITE_NORMAL] = &registry_sprites[REGISTRY_TEXTURE_BLACK_CAT],
			},
			.exec = black_cat_strat
		},
		.entity = {
			.ch = &entities[0].ch
		}
	},

	/* == Team == */
	{
		.ch = {
			.name = "Molko",
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
		.entity = {
			.ch = &entities[1].ch
		}
	},
};

static struct drawable *drawables[16];
static struct drawable_stack drawable_stack;

static struct action *actions[16];
static struct action_stack action_stack;

static struct battle_entity *entities_enemies[1];
static struct battle_entity *entities_team[1];
static struct battle bt;
static struct battle_bar_default default_bar;
static struct battle_bar bar;

static void
prepare_to_fight(void)
{
	action_stack_init(&action_stack, actions, UTIL_SIZE(actions));
	drawable_stack_init(&drawable_stack, drawables, UTIL_SIZE(drawables));

	battle_init(&bt);
	battle_bar_default_init(&default_bar);
	battle_bar_default(&default_bar, &bar);

	bt.team = entities_team;
	bt.teamsz = UTIL_SIZE(entities_team);

	bt.enemies = entities_enemies;
	bt.enemiesz = UTIL_SIZE(entities_enemies);

	/* Black cat is near the previous monster. */
	entities_team[0] = &entities[1].entity;
	entities_enemies[0] = &entities[0].entity;
	entities_enemies[0]->x = 500;
	entities_enemies[0]->y = 100;

	bt.background = &registry_images[REGISTRY_IMAGE_BATTLE_BACKGROUND];
	bt.bar = &bar;
	bt.actions = &action_stack;
	bt.effects = &drawable_stack;

	battle_start(&bt);
	game_push(&fight_state);
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
	(void)st;

	battle_handle(&bt, ev);
}

static void
fight_update(struct state *st, unsigned int ticks)
{
	(void)st;

	if (battle_update(&bt, ticks))
		game_pop();
}

static void
fight_draw(struct state *st)
{
	(void)st;

	painter_set_color(0x000000ff);
	painter_clear();
	battle_draw(&bt);
	painter_present();
}

static void
fight_finish(struct state *st)
{
	(void)st;

	battle_bar_finish(&bar, &bt);
	battle_bar_default_finish(&default_bar);
	battle_finish(&bt);
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
	game_init(states, UTIL_SIZE(states));
	game_push(&empty_state);
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

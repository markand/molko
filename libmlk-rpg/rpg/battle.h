/*
 * battle.h -- battles
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

#ifndef MOLKO_RPG_BATTLE_H
#define MOLKO_RPG_BATTLE_H

#include <stdbool.h>

#include <core/action.h>
#include <core/drawable.h>

#include <ui/frame.h>
#include <ui/gridmenu.h>

#include "battle-bar.h"
#include "battle-entity.h"
#include "battle-state.h"
#include "selection.h"
#include "spell.h"

union event;

struct character;
struct inventory;
struct music;
struct selection;
struct spell;
struct theme;

#define BATTLE_TEAM_MAX         (4)
#define BATTLE_ENEMY_MAX        (8)
#define BATTLE_ENTITY_MAX       (BATTLE_TEAM_MAX + BATTLE_ENEMY_MAX)

#define BATTLE_TEAM_FOREACH(bt, iter) \
	for (size_t i = 0; i < BATTLE_TEAM_MAX && ((iter) = &(bt)->team[i]); ++i)
#define BATTLE_ENEMY_FOREACH(bt, iter) \
	for (size_t i = 0; i < BATTLE_ENEMY_MAX && ((iter) = &(bt)->enemies[i]); ++i)

#define BATTLE_THEME(bt) ((bt)->theme ? (bt)->theme : theme_default())

enum battle_status {
	BATTLE_STATUS_NONE,
	BATTLE_STATUS_RUNNING,
	BATTLE_STATUS_WON,
	BATTLE_STATUS_LOST,
};

struct battle {
	struct battle_state *state;
	enum battle_status status;
	struct battle_entity team[BATTLE_TEAM_MAX];
	struct battle_entity enemies[BATTLE_ENEMY_MAX];
	struct battle_entity *order[BATTLE_ENTITY_MAX];
	struct battle_entity *order_cur;
	size_t order_curindex;
	struct texture *background;
	struct music *music[3];
	struct theme *theme;
	struct drawable_stack effects;
	struct action_stack actions[2];
	struct inventory *inventory;
	struct battle_bar bar;
};

void
battle_start(struct battle *bt);

void
battle_next(struct battle *bt);

struct battle_entity *
battle_find(struct battle *bt, const struct character *ch);

void
battle_switch(struct battle *bt, struct battle_state *st);

void
battle_order(struct battle *bt);

void
battle_attack(struct battle *bt,
              struct character *source,
              struct character *target);

void
battle_cast(struct battle *bt,
            struct character *source,
            const struct spell *spell,
            const struct selection *slt);

void
battle_indicator_hp(struct battle *bt, const struct character *target, unsigned int amount);

void
battle_handle(struct battle *bt, const union event *ev);

bool
battle_update(struct battle *bt, unsigned int ticks);

void
battle_draw(struct battle *bt);

void
battle_finish(struct battle *bt);

#endif /* MOLKO_RPG_BATTLE_H */

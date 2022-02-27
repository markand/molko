/*
 * battle.h -- battles
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_BATTLE_H
#define MLK_RPG_BATTLE_H

#include <core/action.h>
#include <core/action-stack.h>
#include <core/core.h>
#include <core/drawable.h>
#include <core/drawable-stack.h>

#include <ui/frame.h>
#include <ui/gridmenu.h>

#include "battle-entity.h"
#include "battle-state.h"
#include "selection.h"
#include "spell.h"

union event;

struct battle_bar;
struct character;
struct inventory;
struct item;
struct music;
struct selection;
struct spell;
struct theme;

#define BATTLE_TEAM_FOREACH(bt, iter) \
	for (size_t i = 0; i < (bt)->teamsz && ((iter) = (bt)->team[i]); ++i)
#define BATTLE_ENEMY_FOREACH(bt, iter) \
	for (size_t i = 0; i < (bt)->enemiesz && ((iter) = (bt)->enemies[i]); ++i)

#define BATTLE_THEME(bt) ((bt)->theme ? (bt)->theme : theme_default())

enum battle_status {
	BATTLE_STATUS_NONE,
	BATTLE_STATUS_RUNNING,
	BATTLE_STATUS_WON,
	BATTLE_STATUS_LOST,
};

enum battle_component {
	BATTLE_COMPONENT_BACKGROUND     = (1 << 0),
	BATTLE_COMPONENT_ENTITIES       = (1 << 1),
	BATTLE_COMPONENT_BAR            = (1 << 2),
	BATTLE_COMPONENT_ACTIONS        = (1 << 3),
	BATTLE_COMPONENT_DRAWABLES      = (1 << 4),
	BATTLE_COMPONENT_ALL            = 0xff
};

struct battle {
	struct battle_state *state;
	enum battle_status status;
	struct battle_entity **team;
	size_t teamsz;
	struct battle_entity **enemies;
	size_t enemiesz;
	struct battle_entity **order;
	struct battle_entity **ordercur;
	size_t ordersz;
	struct texture *background;
	struct music *music[3];
	struct theme *theme;
	struct drawable_stack *effects;
	struct action_stack *actions;
	struct inventory *inventory;
	struct battle_bar *bar;
};

CORE_BEGIN_DECLS

void
battle_init(struct battle *);

int
battle_ok(const struct battle *);

void
battle_start(struct battle *);

void
battle_next(struct battle *);

struct battle_entity *
battle_find(struct battle *, const struct character *);

void
battle_switch(struct battle *, struct battle_state *);

void
battle_order(struct battle *);

struct battle_entity *
battle_current(const struct battle *);

size_t
battle_index(const struct battle *);

void
battle_attack(struct battle *, struct character *, struct character *);

void
battle_cast(struct battle *, struct character *, const struct spell *, const struct selection *);

void
battle_use(struct battle *, const struct item *, struct character *, struct character *);

void
battle_indicator_hp(struct battle *, const struct character *, long);

void
battle_handle_component(struct battle *, const union event *, enum battle_component);

void
battle_handle(struct battle *, const union event *);

void
battle_update_component(struct battle *, unsigned int, enum battle_component);

int
battle_update(struct battle *, unsigned int);

void
battle_draw_component(const struct battle *, enum battle_component);

void
battle_draw(const struct battle *);

void
battle_finish(struct battle *);

CORE_END_DECLS

#endif /* MLK_RPG_BATTLE_H */

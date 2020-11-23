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

#ifndef MOLKO_BATTLE_H
#define MOLKO_BATTLE_H

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
struct music;
struct spell;
struct theme;

#define BATTLE_TEAM_MAX         (4)     /*!< Maximum team members. */
#define BATTLE_ENEMY_MAX        (8)     /*!< Maximum enemies. */
#define BATTLE_ENTITY_MAX \
	(BATTLE_TEAM_MAX + BATTLE_ENEMY_MAX)

#define BATTLE_TEAM_FOREACH(bt, iter) \
	for (size_t i = 0; i < BATTLE_TEAM_MAX && ((iter) = &(bt)->team[i]); ++i)
#define BATTLE_ENEMY_FOREACH(bt, iter) \
	for (size_t i = 0; i < BATTLE_ENEMY_MAX && ((iter) = &(bt)->enemies[i]); ++i)

#define BATTLE_THEME(bt) ((bt)->theme ? (bt)->theme : theme_default())

/**
 * \brief Generic battle status.
 */
enum battle_status {
	BATTLE_STATUS_NONE,     /*!< Battle is not even started. */
	BATTLE_STATUS_RUNNING,  /*!< Battle is running. */
	BATTLE_STATUS_WON,      /*!< Battle has been won. */
	BATTLE_STATUS_LOST,     /*!< Battle has been lost. */
};

/**
 * \brief Battle structure.
 */
struct battle {
	/**
	 * (+&?) Battle state.
	 */
	struct battle_state *state;

	/**
	 * (-) Battle status.
	 *
	 * This information is provided as final result once the battle has
	 * completed, modifying by the user won't change the battle routines.
	 */
	enum battle_status status;

	/**
	 * (+) Member of team.
	 */
	struct battle_entity team[BATTLE_TEAM_MAX];

	/**
	 * (+) Ennemies to fight.
	 */
	struct battle_entity enemies[BATTLE_ENEMY_MAX];

	/**
	 * (+&?) Order of playing.
	 */
	struct battle_entity *order[BATTLE_ENTITY_MAX];

	/**
	 * (-&?) Current entity playing.
	 */
	struct battle_entity *order_cur;

	/**
	 * (-) Current entity playing index.
	 */
	size_t order_curindex;

	/**
	 * (+&?) An optional background.
	 */
	struct texture *background;

	/**
	 * (+&?) A music to play.
	 *
	 * Music to play in the battle:
	 *
	 * - [0]: while the battle is running,
	 * - [1]: in case of victory,
	 * - [2]: in case of lost.
	 */
	struct music *music[3];

	/**
	 * (+&?) Theme to use.
	 */
	struct theme *theme;

	/**
	 * (+) Stack of animations.
	 *
	 * The drawing animations are ran in parallel.
	 */
	struct drawable_stack effects;

	/**
	 * (+) Stack of actions.
	 *
	 * The array [0] contains actions that must complete to continue the
	 * battle, it can be used to write some messages while blocking the
	 * battle.
	 *
	 * The array [1] is a set of actions that run in "parallel" and don't
	 * prevent the battle from continuing.
	 */
	struct action_stack actions[2];

	/**
	 * (-) Bottom bar.
	 */
	struct battle_bar bar;
};

/**
 * Call this function before using the battle but after calling \a battle_init.
 *
 * If the team entities are positioned to 0, 0 they will be placed
 * automatically.
 *
 * \pre bt != NULL
 * \param bt the battle object
 */
void
battle_start(struct battle *bt);

/**
 * Select next member in battle.
 *
 * \pre bt != NULL
 * \param bt the battle object
 */
void
battle_next(struct battle *bt);

struct battle_entity *
battle_find(struct battle *bt, const struct character *ch);

/**
 * Change battle state.
 *
 * \pre bt != NULL
 * \pre st != NULL
 * \param bt the battle object
 * \param st the state (referenced)
 * \warning This will immediately close the current state and call finish field
 *          function if defined.
 */
void
battle_switch(struct battle *bt, struct battle_state *st);

/**
 * Compute battle ordering.
 *
 * \pre bt != NULL
 * \param bt the battle object
 */
void
battle_order(struct battle *bt);

/**
 * All in one function to animate and compute damage.
 *
 * Use this function from the character AI or when a character from the team
 * should attack an enemy.
 *
 * This will do the following in order:
 *
 * 1. Change battle state to "attacking"
 * 2. Change battle entity state to "moving" or "blinking" if source is member of
 *    team or enemies respectively.
 * 3. Wait until animations complete.
 * 4. Compute and produce damage.
 * 5. Change battle state to "check"
 *
 * \pre bt != NULL
 * \pre source != NULL
 * \pre target != NULL
 * \param bt the battle object
 * \param source the entity attacking
 * \param target the target entity
 */
void
battle_attack(struct battle *bt,
              struct character *source,
              struct character *target);

/**
 * Default function to cast a spell.
 *
 * Prefer to use this function to cast a spell because it performs some checks
 * and hooks before casting the spell.
 *
 * \pre bt != NULL
 * \pre source != NULL
 * \pre spell != NULL
 * \param bt the current battle
 * \param source the entity casting a spell
 * \param spell the spell used
 * \param selection the selection
 */
void
battle_cast(struct battle *bt,
            struct character *source,
            const struct spell *spell,
            unsigned int selection);

/**
 * Spawn an indicator drawable to show damage.
 *
 * The drawable will draw the amount near the entity and fade away after
 * several seconds.
 *
 * \pre bt != NULL
 * \pre target != NULL
 * \param bt the battle object
 * \param target the target entity
 * \param amount the amount of damage
 */
void
battle_indicator_hp(struct battle *bt, const struct character *target, unsigned int amount);

/**
 * Handle an event.
 *
 * \pre bt != NULL
 * \pre ev != NULL
 * \param bt the battle object
 * \param ev the event
 */
void
battle_handle(struct battle *bt, const union event *ev);

/**
 * Update the battle.
 *
 * \pre bt != NULL
 * \param bt the battle object
 * \param ticks the number of milliseconds since last frame
 */
bool
battle_update(struct battle *bt, unsigned int ticks);

/**
 * Draw the battle.
 *
 * \pre bt != NULL
 * \param bt the battle object
 */
void
battle_draw(struct battle *bt);

/**
 * Finish and dispose resources if necessary.
 *
 * \pre bt != NULL
 * \param bt the battle object
 */
void
battle_finish(struct battle *bt);

#endif /* MOLKO_BATTLE_H */

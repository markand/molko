/*
 * selection.c -- kind of selection
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

#include <core/util.h>

#include "battle.h"
#include "character.h"
#include "selection.h"

static void
random(struct selection *slt, const struct battle *bt, const struct battle_entity *entities, size_t entitiesz)
{
	(void)bt;

	struct {
		const struct battle_entity *entity;
		size_t position;
	} table[BATTLE_ENTITY_MAX] = {0};

	size_t tablesz = 0;

	/*
	 * Merge the list of valid entities into the table to select a random
	 * one.
	 */
	for (size_t i = 0; i < entitiesz; ++i) {
		if (battle_entity_ok(&entities[i])) {
			table[tablesz].entity = &entities[i];
			table[tablesz++].position = i;
		}
	}

	slt->index_character = table[util_nrand(0, tablesz)].position;
}

static void
first(struct selection *slt, const struct battle *bt, const struct battle_entity *entities, size_t entitiesz)
{
	(void)bt;

	for (size_t i = 0; i < entitiesz; ++i) {
		if (battle_entity_ok(&entities[i])) {
			slt->index_character = i;
			break;
		}
	}
}

void
selection_first(struct selection *slt, const struct battle *bt)
{
	assert(slt);
	assert(bt);

	if (slt->index_side == 0)
		first(slt, bt, bt->enemies, BATTLE_ENEMY_MAX);
	else
		first(slt, bt, bt->team, BATTLE_TEAM_MAX);
}

void
selection_random(struct selection *slt, const struct battle *bt)
{
	assert(slt);
	assert(bt);

	if (slt->index_side == 0)
		random(slt, bt, bt->enemies, BATTLE_ENEMY_MAX);
	else
		random(slt, bt, bt->team, BATTLE_TEAM_MAX);
}

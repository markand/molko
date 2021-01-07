/*
 * battle-state.h -- battle abstract state
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

#ifndef MOLKO_RPG_BATTLE_STATE_H
#define MOLKO_RPG_BATTLE_STATE_H

/**
 * \file battle-state.h
 * \brief Battle abstract state.
 *
 * As a game battle is split into different steps, they are implemented as
 * multiple states to facilitate transitions and have a less complicated code.
 *
 * Each state can handle, update and draw the game logic. The battle itself
 * does only few things by itself like playing music, handling custom actions
 * and drawables and dispatch the rest to the current state.
 */

#include <stdbool.h>

#include "selection.h"

struct battle;
struct character;

union event;

struct battle_state {
	void *data;
	void (*handle)(struct battle_state *st, struct battle *bt, const union event *ev);
	bool (*update)(struct battle_state *st, struct battle *bt, unsigned int ticks);
	void (*draw)(const struct battle_state *st, const struct battle *bt);
	void (*finish)(struct battle_state *st, struct battle *bt);
};

void
battle_state_handle(struct battle_state *st, struct battle *bt, const union event *ev);

bool
battle_state_update(struct battle_state *st, struct battle *bt, unsigned int ticks);

void
battle_state_draw(const struct battle_state *st, const struct battle *bt);

void
battle_state_finish(struct battle_state *st, struct battle *bt);

/* States switchers, defined in their own files. */
void
battle_state_ai(struct battle *bt);

void
battle_state_attacking(struct battle *bt, struct character *source, struct character *target);

void
battle_state_check(struct battle *bt);

void
battle_state_check(struct battle *bt);

void
battle_state_closing(struct battle *bt);

void
battle_state_lost(struct battle *bt);

void
battle_state_menu(struct battle *bt);

void
battle_state_opening(struct battle *bt);

void
battle_state_selection(struct battle *bt, const struct selection *slt);

void
battle_state_sub(struct battle *bt);

void
battle_state_victory(struct battle *bt);

#endif /* !MOLKO_RPG_BATTLE_STATE_H */

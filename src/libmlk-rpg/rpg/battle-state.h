/*
 * battle-state.h -- battle abstract state
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

#ifndef MLK_RPG_BATTLE_STATE_H
#define MLK_RPG_BATTLE_STATE_H

#include <core/core.h>

struct battle;
struct character;
struct inventory_slot;
struct selection;

union event;

struct battle_state {
	void *data;
	void (*handle)(struct battle_state *, struct battle *, const union event *);
	int (*update)(struct battle_state *, struct battle *, unsigned int);
	void (*draw)(const struct battle_state *, const struct battle *);
	void (*finish)(struct battle_state *, struct battle *);
};

CORE_BEGIN_DECLS

void
battle_state_handle(struct battle_state *, struct battle *, const union event *);

int
battle_state_update(struct battle_state *, struct battle *, unsigned int);

void
battle_state_draw(const struct battle_state *, const struct battle *);

void
battle_state_finish(struct battle_state *, struct battle *);

/* States switchers, defined in their own files. */
void
battle_state_ai(struct battle *);

void
battle_state_attacking(struct battle *, struct character *, struct character *);

void
battle_state_item(struct battle *,
                  struct character *,
                  struct character *,
                  struct inventory_slot *);

void
battle_state_check(struct battle *);

void
battle_state_closing(struct battle *);

void
battle_state_lost(struct battle *);

void
battle_state_menu(struct battle *);

void
battle_state_opening(struct battle *);

void
battle_state_selection(struct battle *, const struct selection *);

void
battle_state_sub(struct battle *);

void
battle_state_victory(struct battle *);

CORE_END_DECLS

#endif /* !MLK_RPG_BATTLE_STATE_H */

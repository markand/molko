/*
 * battle-state-selection.h -- battle state (selection)
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

#ifndef MLK_RPG_BATTLE_STATE_SELECTION_H
#define MLK_RPG_BATTLE_STATE_SELECTION_H

#include <rpg/selection.h>

struct battle;

union event;

struct battle_state_selection {
	struct selection select;
};

void
battle_state_selection_init(struct battle_state_selection *, const struct selection *);

void
battle_state_selection_handle(struct battle_state_selection *, struct battle *, const union event *);

void
battle_state_selection_draw(const struct battle_state_selection *, const struct battle *);

void
battle_state_selection(struct battle *, const struct selection *);

#endif /* !MLK_RPG_BATTLE_STATE_SELECTION_H */

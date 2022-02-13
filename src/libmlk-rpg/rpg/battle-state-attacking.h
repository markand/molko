/*
 * battle-state-attacking.h -- battle state (entity is moving for attacking)
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

#ifndef MLK_RPG_BATTLE_STATE_ATTACKING_H
#define MLK_RPG_BATTLE_STATE_ATTACKING_H

struct battle;
struct battle_entity;

enum battle_state_attacking_status {
	/* For team. */
	BATTLE_STATE_ATTACKING_ADVANCING,
	BATTLE_STATE_ATTACKING_DAMAGING,
	BATTLE_STATE_ATTACKING_RETURNING,

	/* For enemies. */
	BATTLE_STATE_ATTACKING_BLINKING,
};

struct battle_state_attacking {
	enum battle_state_attacking_status status;
	struct battle_entity *source;
	struct battle_entity *target;
	int origin_x;
	int origin_y;
};

void
battle_state_attacking_init(struct battle_state_attacking *,
                            struct battle_entity *,
			    struct battle_entity *);

int
battle_state_attacking_update(struct battle_state_attacking *, struct battle *);

void
battle_state_attacking(struct battle_entity *, struct battle_entity *, struct battle *);

#endif /* !#define MLK_RPG_BATTLE_STATE_ATTACKING_H */

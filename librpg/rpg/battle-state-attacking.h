/*
 * battle-state-attacking.h -- battle state (entity is moving for attacking)
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

#ifndef MOLKO_BATTLE_STATE_ATTACKING_H
#define MOLKO_BATTLE_STATE_ATTACKING_H

/**
 * \file battle-state-attacking.h
 * \brief Battle state (entity is moving for attacking).
 *
 * This state will move the entity source to the target if it is a member of
 * team or will blink the enemy. Then it computes damage and change state to
 * check.
 */

struct battle;
struct character;

/**
 * Switch to battle state attacking.
 *
 * \pre battle_ok(bt)
 * \pre character_ok(source)
 * \pre character_ok(target)
 * \param bt the battle to change state
 * \param source the entity attacking
 * \param target the target to damage
 * \post bt->state->data is set to a custom data type
 * \post bt->state->update is set
 * \post bt->state->finish is set
 */
void
battle_state_attacking(struct battle *bt, struct character *source, struct character *target);

#endif /* !MOLKO_BATTLE_STATE_CHECK_H */

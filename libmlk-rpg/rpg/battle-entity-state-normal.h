/*
 * battle-entity-state-normal.h -- the entity is normal
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

#ifndef MOLKO_RPG_BATTLE_ENTITY_STATE_NORMAL_H
#define MOLKO_RPG_BATTLE_ENTITY_STATE_NORMAL_H

/**
 * \file battle-entity-state-normal.h
 * \brief The entity is normal.
 */

struct battle_entity;

/**
 * Set normal state.
 *
 * The normal state does nothing except rendering the entity at its current
 * position.
 *
 * \pre battle_entity_ok(et)
 * \param et the entity to set
 */
void
battle_entity_state_normal(struct battle_entity *et);

#endif /* !MOLKO_RPG_BATTLE_ENTITY_STATE_NORMAL_H */

/*
 * battle-state-menu.h -- battle state (menu)
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

#ifndef MOLKO_RPG_BATTLE_STATE_MENU_H
#define MOLKO_RPG_BATTLE_STATE_MENU_H

/**
 * \file battle-state-menu.h
 * \brief Battle state (menu).
 */

struct battle;

/**
 * Switch to battle state menu.
 *
 * \pre bt != NULL
 * \param bt the battle to change state
 * \post bt->state->handle is set
 */
void
battle_state_menu(struct battle *bt);

#endif /* !MOLKO_RPG_BATTLE_STATE_MENU_H */

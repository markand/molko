/*
 * battle-state-selection.h -- battle state (selection)
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

#ifndef MOLKO_BATTLE_STATE_SELECTION_H
#define MOLKO_BATTLE_STATE_SELECTION_H

/**
 * \file battle-state-selection.h
 * \brief Battle state (selection).
 */

#include "selection.h"

struct battle;

/**
 * Switch to battle state selection.
 *
 * \pre bt != NULL
 * \param bt the battle to change state
 * \param type the kind of selection allowed
 * \param selection the current selection
 * \post bt->state->data is not NULL
 * \post bt->state->handle is set
 * \post bt->state->draw is set
 * \post bt->state->finish is set
 */
void
battle_state_selection(struct battle *bt,
                       enum selection type,
                       unsigned int selection);

#endif /* !MOLKO_BATTLE_STATE_SELECTION_H */

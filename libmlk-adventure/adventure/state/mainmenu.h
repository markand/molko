/*
 * mainmenu.h -- game main menu
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

#ifndef MOLKO_ADVENTURE_STATE_MAINMENU_H
#define MOLKO_ADVENTURE_STATE_MAINMENU_H

/**
 * \file mainmenu.h
 * \brief Game main menu.
 */

/**
 * Create a state about the main game menu.
 *
 * \pre state != NULL
 * \param state the state to initialize
 * \post state->data is set internal data
 * \post state->handle is set
 * \post state->draw is set
 * \post state->update is set
 * \post state->finish is set
 */
void
mainmenu_state(struct state *state);

#endif /* !MOLKO_ADVENTURE_STATE_PANIC_H */

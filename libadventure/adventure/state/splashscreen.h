/*
 * splashscreen.h -- splash screen state
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

#ifndef MOLKO_STATE_SPLASHSCREEN_H
#define MOLKO_STATE_SPLASHSCREEN_H

/**
 * \file splashscreen.h
 * \brief Splash screen state.
 */

/**
 * Create a state for showing a splashcreen.
 *
 * \pre state != NULL
 * \pre next != NULL
 * \param state the state to initialize
 * \param next the next state once it's finished
 * \post state->data is set internal data
 * \post state->draw is set
 * \post state->update is set
 * \post state->finish is set
 */
void
splashscreen_state(struct state *state, struct state *next);

#endif /* !MOLKO_STATE_SPLASHSCREEN_H */

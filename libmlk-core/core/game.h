/*
 * game.h -- main game object
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

#ifndef MOLKO_CORE_GAME_H
#define MOLKO_CORE_GAME_H

#include <stdbool.h>

#include "inhibit.h"

struct state;

union event;

struct game {
	/* Inhibition */
	enum inhibit inhibit;

	/* Game states. */
	struct state *state;
	struct state *state_next;
};

extern struct game game;

void
game_switch(struct state *state, bool quick);

struct state *
game_replace(struct state *state);

void
game_handle(const union event *event);

void
game_update(unsigned int ticks);

void
game_draw(void);

void
game_loop(void);

void
game_quit(void);

#endif /* !MOLKO_CORE_GAME_H */

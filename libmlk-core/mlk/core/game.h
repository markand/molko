/*
 * game.h -- main game object
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_GAME_H
#define MLK_CORE_GAME_H

#include <stddef.h>

#include "core.h"
#include "inhibit.h"

struct state;

union mlk_event;

struct mlk_game {
	enum mlk_inhibit inhibit;
	struct state **states;
	size_t statesz;
	struct state **state;
};

extern struct mlk_game game;

MLK_CORE_BEGIN_DECLS

void
mlk_game_init(struct state **, size_t);

void
mlk_game_push(struct state *);

void
mlk_game_pop(void);

void
mlk_game_handle(const union mlk_event *);

void
mlk_game_update(unsigned int);

void
mlk_game_draw(void);

void
mlk_game_loop(void);

void
mlk_game_quit(void);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_GAME_H */

/*
 * molko.h -- main structure for Molko's Adventure
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

#ifndef MOLKO_MOLKO_H
#define MOLKO_MOLKO_H

#include <core/game.h>
#include <core/texture.h>
#include <core/sprite.h>
#include <core/state.h>

#include <rpg/map-file.h>
#include <rpg/map.h>

enum molko_state {
	MOLKO_STATE_SPLASH,
	MOLKO_STATE_MAINMENU,
	MOLKO_STATE_PANIC,
	MOLKO_STATE_MAP,
	MOLKO_STATE_NUM
};

struct molko {
	struct game engine;
	struct state states[MOLKO_STATE_NUM];

	/* MOLKO_STATE_MAP. */
	struct texture map_player_texture;
	struct sprite map_player_sprite;
	struct map_file map_file;
	struct map map;
};

extern struct molko molko;

void
molko_init(void);

void
molko_run(void);

void
molko_finish(void);

#endif /* !MOLKO_MOLKO_H */

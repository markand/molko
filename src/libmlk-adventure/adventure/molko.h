/*
 * molko.h -- main structure for Molko's Adventure
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#ifndef MOLKO_ADVENTURE_MOLKO_H
#define MOLKO_ADVENTURE_MOLKO_H

#include <core/core.h>
#include <core/game.h>
#include <core/texture.h>
#include <core/sprite.h>

#include <rpg/inventory.h>
#include <rpg/team.h>

struct battle;

enum molko_state {
	MOLKO_STATE_MAP,
	MOLKO_STATE_BATTLE,
};

struct molko {
	struct state *panic;

	/* Make sure to set this accordingly when changing states. */
	enum molko_state state;

	struct team team;
	struct inventory inventory;

	/* For map state. */
	struct texture map_player_texture;
	struct sprite map_player_sprite;
};

CORE_BEGIN_DECLS

extern struct molko molko;

void
molko_init(void);

void
molko_run(void);

void
molko_teleport(const char *map, int origin_x, int origin_y);

void
molko_fight(struct battle *bt);

const char *
molko_path(const char *file);

void
molko_finish(void);

CORE_END_DECLS

#endif /* !MOLKO_ADVENTURE_MOLKO_H */
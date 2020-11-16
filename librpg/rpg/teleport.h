/*
 * teleport.h -- animate and teleport
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

#ifndef MOLKO_RPG_TELEPORT_H
#define MOLKO_RPG_TELEPORT_H

/**
 * \file teleport.h
 * \brief Animate screen and teleport.
 */

#include <stdbool.h>

#include <core/texture.h>

struct action;
struct map;
struct state;

struct teleport {
	struct state *state;
	unsigned int elapsed;
	unsigned int alpha;
	struct texture fade;
};

void
teleport_start(struct teleport *tp);

bool
teleport_update(struct teleport *tp, unsigned int ticks);

void
teleport_draw(struct teleport *tp);

void
teleport_finish(struct teleport *tp);

void
teleport_action(struct teleport *tp, struct action *act);

#endif /* !MOLKO_RPG_TELEPORT_H */

/*
 * spawner.h -- spawn battle while moving
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

#ifndef MOLKO_ADVENTURE_ACTION_SPAWNER_H
#define MOLKO_ADVENTURE_ACTION_SPAWNER_H

#include <core/action.h>
#include <core/core.h>

struct map;

struct spawner {
	struct map *map;
	unsigned int low;
	unsigned int high;
	unsigned int steps;
	struct action action;
	int last_x;
	int last_y;
};

CORE_BEGIN_DECLS

void
spawner_init(struct spawner *);

struct action *
spawner_action(struct spawner *);

CORE_END_DECLS

#endif /* !MOLKO_ADVENTURE_ACTION_SPAWNER_H */

/*
 * chest.h -- chest object
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

#ifndef MOLKO_ADVENTURE_ACTION_CHEST_H
#define MOLKO_ADVENTURE_ACTION_CHEST_H

#include <core/action.h>
#include <core/animation.h>

struct map;
struct sound;

enum chest_state {
	CHEST_STATE_CLOSED,
	CHEST_STATE_ANIMATE,
	CHEST_STATE_OPEN
};

struct chest {
	/* Mandatory. */
	int x;
	int y;
	struct map *map;
	struct animation animation;

	/* Defaulted. */
	enum chest_state state;
	struct action action;

	/* Optional. */
	struct save *save;
	struct save_property *property;
	struct sound *sound;
	void *data;
	void (*exec)(struct chest *);
};

void
chest_init(struct chest *c);

struct action *
chest_action(struct chest *c);

#endif /* !MOLKO_ADVENTURE_ACTION_CHEST_H */

/*
 * battle-bar.h -- abstract battle bar
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_BATTLE_BAR_H
#define MLK_RPG_BATTLE_BAR_H

#include <core/core.h>

struct battle;
struct selection;

union event;

struct battle_bar {
	void *data;
	void (*start)(struct battle_bar *, struct battle *);
	void (*select)(struct battle_bar *, struct battle *, const struct selection *);
	void (*handle)(struct battle_bar *, struct battle *, const union event *);
	void (*update)(struct battle_bar *, struct battle *, unsigned int);
	void (*draw)(const struct battle_bar *, const struct battle *);
	void (*finish)(struct battle_bar *, struct battle *);
};

CORE_BEGIN_DECLS

void
battle_bar_start(struct battle_bar *, struct battle *);

void
battle_bar_select(struct battle_bar *, struct battle *, const struct selection *);

void
battle_bar_handle(struct battle_bar *, struct battle *, const union event *);

void
battle_bar_update(struct battle_bar *, struct battle *, unsigned int);

void
battle_bar_draw(const struct battle_bar *, const struct battle *);

void
battle_bar_finish(struct battle_bar *, struct battle *);

CORE_END_DECLS

#endif /* !MLK_RPG_BATTLE_BAR_H */

/*
 * state.h -- abstract state
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

#ifndef MOLKO_CORE_STATE_H
#define MOLKO_CORE_STATE_H

#include "core.h"

union event;

struct state {
	void *data;
	void (*start)(struct state *state);
	void (*handle)(struct state *state, const union event *ev);
	void (*update)(struct state *state, unsigned int ticks);
	void (*draw)(struct state *state);
	void (*end)(struct state *state);
	void (*finish)(struct state *state);
};

CORE_BEGIN_DECLS

void
state_start(struct state *state);

void
state_handle(struct state *state, const union event *ev);

void
state_update(struct state *state, unsigned int ticks);

void
state_draw(struct state *state);

void
state_end(struct state *state);

void
state_finish(struct state *state);

CORE_END_DECLS

#endif /* !MOLKO_CORE_STATE_H */

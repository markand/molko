/*
 * wait.c -- wait action
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "action.h"
#include "wait.h"
#include "util.h"

static bool
update(struct action *a, unsigned int ticks)
{
	assert(a);

	return wait_update(a->data, ticks);
}

void
wait_start(struct wait *w)
{
	assert(w);

	w->elapsed = 0u;
}

bool
wait_update(struct wait *w, unsigned int ticks)
{
	assert(w);

	w->elapsed += ticks;

	if (w->elapsed >= w->delay)
		w->elapsed = w->delay;

	return w->elapsed >= w->delay;
}

void
wait_action(struct wait *w, struct action *a)
{
	assert(w);
	assert(a);

	memset(a, 0, sizeof (struct action));

	a->data = w;
	a->update = update;
}

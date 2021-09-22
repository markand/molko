/*
 * script.h -- convenient sequence of actions
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

#ifndef MOLKO_CORE_SCRIPT_H
#define MOLKO_CORE_SCRIPT_H

#include <stddef.h>

#include "core.h"

#define SCRIPT_ACTION_MAX (128)

struct action;

union event;

struct script {
	struct action *actions[SCRIPT_ACTION_MAX];
	size_t actionsz;
	size_t cur;
};

CORE_BEGIN_DECLS

void
script_init(struct script *);

int
script_append(struct script *, struct action *);

void
script_handle(struct script *, const union event *);

int
script_update(struct script *, unsigned int);

void
script_draw(struct script *);

int
script_completed(const struct script *);

void
script_finish(struct script *);

void
script_action(struct script*s, struct action *);

CORE_END_DECLS

#endif /* !MOLKO_CORE_SCRIPT_H */

/*
 * battle-message.h -- automatic top center message in battle
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

#ifndef MOLKO_ADVENTURE_BATTLE_MESSAGE_H
#define MOLKO_ADVENTURE_BATTLE_MESSAGE_H

#include <stdbool.h>

#include <core/core.h>

struct theme;

struct battle_message {
	const char *text;
	struct theme *theme;
	unsigned int elapsed;
};

CORE_BEGIN_DECLS

bool
battle_message_update(struct battle_message *, unsigned int);

void
battle_message_draw(const struct battle_message *);

CORE_END_DECLS

#endif /* !MOLKO_ADVENTURE_BATTLE_MESSAGE_H */

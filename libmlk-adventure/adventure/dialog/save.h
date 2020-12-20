/*
 * save.h -- select a save slot
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

#ifndef MOLKO_ADVENTURE_SAVE_H
#define MOLKO_ADVENTURE_SAVE_H

#include <stdbool.h>

#include <rpg/save.h>

#define DIALOG_SAVE_MAX (6)

union event;

struct theme;

struct dialog_save {
	const struct theme *theme;
	struct save saves[DIALOG_SAVE_MAX];
	size_t selected;
};

void
dialog_save_init(struct dialog_save *);

bool
dialog_save_handle(struct dialog_save *, const union event *);

void
dialog_save_update(struct dialog_save *, unsigned int);

void
dialog_save_draw(const struct dialog_save *);

void
dialog_save_finish(struct dialog_save *);

#endif /* !MOLKO_ADVENTURE_SAVE_H */

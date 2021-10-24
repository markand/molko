/*
 * quest.h -- in game quests
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

#ifndef MLK_RPG_QUEST_H
#define MLK_RPG_QUEST_H

#include <stddef.h>

#include <core/core.h>

struct save;

struct quest_step {
	const char *name;
	const char *description;
	int percent;
};

struct quest {
	const char *name;
	const char *description;
	struct quest_step *steps;
	size_t stepsz;
};

CORE_BEGIN_DECLS

int
quest_save(struct quest *, struct save *);

int
quest_load(struct quest *, struct save *);

CORE_END_DECLS

#endif /* !MLK_RPG_QUEST_H */

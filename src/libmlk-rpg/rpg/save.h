/*
 * save.h -- save functions
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

#ifndef MOLKO_RPG_SAVE_H
#define MOLKO_RPG_SAVE_H

#include <time.h>

#include <core/core.h>

#define SAVE_PROPERTY_KEY_MAX   (64)
#define SAVE_PROPERTY_VALUE_MAX (1024)

struct save {
	time_t created;
	time_t updated;
	void *handle;
};

enum save_mode {
	SAVE_MODE_READ,
	SAVE_MODE_WRITE
};

struct save_property {
	char key[SAVE_PROPERTY_KEY_MAX + 1];
	char value[SAVE_PROPERTY_VALUE_MAX + 1];
};

struct save_stmt {
	struct save *parent;
	void *handle;
};

CORE_BEGIN_DECLS

int
save_open(struct save *, unsigned int, enum save_mode);

int
save_open_path(struct save *, const char *, enum save_mode);

int
save_ok(const struct save *);

int
save_set_property(struct save *, const struct save_property *);

int
save_get_property(struct save *, struct save_property *);

int
save_remove_property(struct save *, const struct save_property *);

int
save_exec(struct save *, const char *, const char *, ...);

void
save_finish(struct save *);

/* Prepared statements. */
int
save_stmt_init(struct save *, struct save_stmt *, const char *, const char *, ...);

int
save_stmt_next(struct save_stmt *, const char *, ...);

void
save_stmt_finish(struct save_stmt *);

CORE_END_DECLS

#endif /* !MOLKO_RPG_SAVE_H */

/*
 * notify.h -- in game notifications
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

#ifndef MOLKO_UI_NOTIFY_H
#define MOLKO_UI_NOTIFY_H

#include <stddef.h>

#include <core/core.h>

#define NOTIFY_MAX              (4)
#define NOTIFY_TIMEOUT_DEFAULT  (5000)

struct texture;
struct theme;

struct notify {
	const struct texture *icon;
	const char *title;
	const char *body;
	unsigned int elapsed;
};

struct notify_system {
	struct theme *theme;
	void (*draw)(const struct notify *, size_t);
};

CORE_BEGIN_DECLS

void
notify(const struct texture *, const char *, const char *);

void
notify_update(unsigned int ticks);

void
notify_draw(void);

void
notify_set_system(const struct notify_system *);

CORE_END_DECLS

#endif /*! MOLKO_UI_NOTIFY_H */
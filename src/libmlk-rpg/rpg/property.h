/*
 * property.h -- manage game properties
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

#ifndef MOLKO_RPG_PROPERTY_H
#define MOLKO_RPG_PROPERTY_H

#include <core/core.h>

#define PROPERTY_KEY_MAX        (64)
#define PROPERTY_VALUE_MAX      (1024)

struct save;

struct property {
	char key[PROPERTY_KEY_MAX + 1];
	char value[PROPERTY_VALUE_MAX + 1];
};

CORE_BEGIN_DECLS

int
property_save(const struct property *, struct save *);

int
property_load(struct property *, struct save *);

int
property_remove(struct property *, struct save *);

CORE_END_DECLS

#endif /* !MOLKO_RPG_PROPERTY_H */
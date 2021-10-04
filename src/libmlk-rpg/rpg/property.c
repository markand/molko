/*
 * property.c -- manage game properties
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

#include <assert.h>

#include <assets/sql/property-save.h>
#include <assets/sql/property-remove.h>
#include <assets/sql/property-load.h>

#include "property.h"
#include "save.h"

int
property_save(const struct property *p, struct save *s)
{
	assert(p);
	assert(save_ok(s));

	return save_exec(s, (const char *)assets_sql_property_save, "ss", p->key, p->value);
}

int
property_load(struct property *p, struct save *s)
{
	assert(p);
	assert(save_ok(s));

	struct save_stmt stmt;
	enum save_stmt_errno ret;

	if (save_stmt_init(&stmt, s, (const char *)assets_sql_property_load, "s", p->key) < 0)
		return -1;

	ret = save_stmt_next(&stmt, "s", p->value, sizeof (p->value)) == SAVE_STMT_ROW;
	save_stmt_finish(&stmt);

	return ret ? 0 : -1;
}

int
property_remove(struct property *p, struct save *s)
{
	assert(p);
	assert(save_ok(s));

	return save_exec(s, (const char *)assets_sql_property_remove, "s", p->key);
}


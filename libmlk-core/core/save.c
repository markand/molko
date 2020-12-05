/*
 * save.c -- save functions
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

#include <compat.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include <assets/sql/init.h>
#include <assets/sql/property-get.h>
#include <assets/sql/property-remove.h>
#include <assets/sql/property-set.h>

#include "core_p.h"
#include "error.h"
#include "save.h"
#include "sys.h"
#include "util.h"

#define SQL_BEGIN       "BEGIN EXCLUSIVE TRANSACTION"
#define SQL_COMMIT      "COMMIT"
#define SQL_ROLLBACK    "ROLLBACK"

static bool
exec(struct save *db, const char *sql)
{
	if (sqlite3_exec(db->handle, sql, NULL, NULL, NULL) != SQLITE_OK)
		return errorf("%s", sqlite3_errmsg(db->handle));

	return true;
}

static const char *
path(unsigned int idx)
{
	return pprintf("%s/%u", sys_dir(SYS_DIR_SAVE), idx);
}

static bool
execu(struct save *db, const unsigned char *sql)
{
	return exec(db, (const char *)sql);
}

bool
save_open(struct save *db, unsigned int idx, enum save_mode mode)
{
	assert(db);

	return save_open_path(db, path(idx), mode);
}

bool
verify(struct save *db)
{
	struct {
		time_t *date;
		struct save_property prop;
	} table[] = {
		{ .date = &db->created, { .key = "molko.create-date" } },
		{ .date = &db->updated, { .key = "molko.update-date" } },
	};

	/* Ensure create and update dates are present. */
	for (size_t i = 0; i < NELEM(table); ++i) {
		if (!save_get_property(db, &table[i].prop)) {
			sqlite3_close(db->handle);
			return errorf(_("database not initialized correctly"));
		}

		*table[i].date = strtoull(table[i].prop.value, NULL, 10);
	}

	return true;
}

bool
save_open_path(struct save *db, const char *path, enum save_mode mode)
{
	assert(db);
	assert(path);

	int flags = 0;

	switch (mode) {
	case SAVE_MODE_WRITE:
		flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		break;
	default:
		flags = SQLITE_OPEN_READONLY;
		break;
	}

	if (sqlite3_open_v2(path, (sqlite3**)&db->handle, flags, NULL) != SQLITE_OK)
		return errorf("%s", sqlite3_errmsg(db->handle));

	if (mode == SAVE_MODE_WRITE) {
		if (!execu(db, sql_init)) {
			sqlite3_close(db->handle);
			return false;
		}
	}

	return verify(db);
}

bool
save_set_property(struct save *db, const struct save_property *prop)
{
	assert(db);
	assert(prop);

	sqlite3_stmt *stmt = NULL;

	if (!exec(db, SQL_BEGIN))
		return false;
	if (sqlite3_prepare(db->handle, (const char *)sql_property_set, -1, &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, prop->key, -1, NULL) != SQLITE_OK ||
	    sqlite3_bind_text(stmt, 2, prop->value, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_step(stmt) != SQLITE_DONE)
		goto sqlite3_err;

	sqlite3_finalize(stmt);

	return exec(db, SQL_COMMIT);

sqlite3_err:
	errorf("%s", sqlite3_errmsg(db->handle));

	if (stmt)
		sqlite3_finalize(stmt);

	exec(db, SQL_ROLLBACK);

	return false;
}

bool
save_get_property(struct save *db, struct save_property *prop)
{
	assert(db);
	assert(prop);

	sqlite3_stmt *stmt = NULL;
	bool ret = true;

	if (sqlite3_prepare(db->handle, (const char *)sql_property_get,
	    sizeof (sql_property_get), &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, prop->key, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;

	switch (sqlite3_step(stmt)) {
	case SQLITE_DONE:
		/* Not found. */
		ret = errorf(_("property '%s' was not found"), prop->key);
		break;
	case SQLITE_ROW:
		/* Found. */
		strlcpy(prop->value, (const char *)sqlite3_column_text(stmt, 0),
		    sizeof (prop->value));
		break;
	default:
		/* Error. */
		goto sqlite3_err;
	}

	sqlite3_finalize(stmt);

	return ret;

sqlite3_err:
	errorf("%s", sqlite3_errmsg(db->handle));

	if (stmt)
		sqlite3_finalize(stmt);

	return false;
}

bool
save_remove_property(struct save *db, const struct save_property *prop)
{
	assert(db);
	assert(prop);

	sqlite3_stmt *stmt = NULL;

	if (!exec(db, SQL_BEGIN))
		return false;
	if (sqlite3_prepare(db->handle, (const char *)sql_property_remove,
	    sizeof (sql_property_remove), &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, prop->key, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_step(stmt) != SQLITE_DONE)
		goto sqlite3_err;

	sqlite3_finalize(stmt);

	return exec(db, SQL_COMMIT);

sqlite3_err:
	errorf("%s", sqlite3_errmsg(db->handle));

	if (stmt)
		sqlite3_finalize(stmt);

	exec(db, SQL_ROLLBACK);

	return false;
}

void
save_finish(struct save *db)
{
	assert(db);

	if (db->handle)
		sqlite3_close(db->handle);

	memset(db, 0, sizeof (*db));
}

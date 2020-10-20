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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>

#include "error.h"
#include "save.h"
#include "sys.h"

static sqlite3 *db;

static const char *sinit =
	"BEGIN EXCLUSIVE TRANSACTION;"
	""
	"CREATE TABLE IF NOT EXISTS property("
	"  id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"  key TEXT NOT NULL UNIQUE,"
	"  value TEXT NOT NULL"
	");"
	""
	"COMMIT"
;

static const char *sbegin =
	"BEGIN EXCLUSIVE TRANSACTION"
;

static const char *scommit =
	"COMMIT"
;

static const char *srollback =
	"ROLLBACK"
;

static const char *sset_property =
	"INSERT OR REPLACE INTO property("
	"  key,"
	"  value"
	")"
	"VALUES("
	"  ?,"
	"  ?"
	");"
;

static const char *sget_property =
	"SELECT value"
	"  FROM property"
	" WHERE key = ?"
;

static const char *sremove_property =
	"DELETE"
	"  FROM property"
	" WHERE key = ?"
;

static bool
exec(const char *sql)
{
	if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
		return errorf("%s", sqlite3_errmsg(db));

	return true;
}

bool
save_open(unsigned int idx)
{
	return save_open_path(sys_savepath(idx));
}

bool
save_open_path(const char *path)
{
	assert(path);

	if (sqlite3_open(path, &db) != SQLITE_OK)
		return errorf("database open error: %s", sqlite3_errmsg(db));
	if (sqlite3_exec(db, sinit, NULL, NULL, NULL) != SQLITE_OK)
		return errorf("database init error: %s", sqlite3_errmsg(db));

	return true;
}

bool
save_set_property(const char *key, const char *value)
{
	assert(key);
	assert(value && strlen(value) <= SAVE_PROPERTY_VALUE_MAX);

	sqlite3_stmt *stmt = NULL;

	if (!exec(sbegin))
		return false;
	if (sqlite3_prepare(db, sset_property, -1, &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, key, -1, NULL) != SQLITE_OK ||
	    sqlite3_bind_text(stmt, 2, value, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_step(stmt) != SQLITE_DONE)
		goto sqlite3_err;

	sqlite3_finalize(stmt);

	return exec(scommit);

sqlite3_err:
	if (stmt) {
		sqlite3_finalize(stmt);
		exec(srollback);
	}

	return errorf("%s", sqlite3_errmsg(db));
}

const char *
save_get_property(const char *key)
{
	assert(key);

	static char value[SAVE_PROPERTY_VALUE_MAX + 1];
	const char *ret = value;
	sqlite3_stmt *stmt = NULL;

	memset(value, 0, sizeof (value));

	if (sqlite3_prepare(db, sget_property, -1, &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, key, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;

	switch (sqlite3_step(stmt)) {
	case SQLITE_DONE:
		/* Not found. */
		ret = NULL;
		break;
	case SQLITE_ROW:
		/* Found. */
		snprintf(value, sizeof (value), "%s", sqlite3_column_text(stmt, 0));
		break;
	default:
		/* Error. */
		goto sqlite3_err;
	}

	sqlite3_finalize(stmt);

	return ret;

sqlite3_err:
	if (stmt)
		sqlite3_finalize(stmt);

	errorf("%s", sqlite3_errmsg(db));

	return NULL;
}

bool
save_remove_property(const char *key)
{
	assert(key);

	sqlite3_stmt *stmt = NULL;

	if (!exec(sbegin))
		return false;
	if (sqlite3_prepare(db, sremove_property, -1, &stmt, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_bind_text(stmt, 1, key, -1, NULL) != SQLITE_OK)
		goto sqlite3_err;
	if (sqlite3_step(stmt) != SQLITE_DONE)
		goto sqlite3_err;

	sqlite3_finalize(stmt);

	return exec(scommit);

sqlite3_err:
	if (stmt)
		sqlite3_finalize(stmt);

	errorf("%s", sqlite3_errmsg(db));

	return false;
}

void
save_finish(void)
{
	if (db)
		sqlite3_close(db);
}

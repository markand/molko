/*
 * save.c -- save functions
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include <mlk/util/util.h>

#include <mlk/core/err.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>

#include <assets/sql/init.h>

#include "property.h"
#include "save.h"

#define SQL_BEGIN       "BEGIN EXCLUSIVE TRANSACTION"
#define SQL_COMMIT      "COMMIT"
#define SQL_ROLLBACK    "ROLLBACK"

static inline int
set_error(struct save *db)
{
	return mlk_errf("%s", sqlite3_errmsg(db->handle));
}

static inline int
exec(struct save *db, const char *sql)
{
	if (sqlite3_exec(db->handle, sql, NULL, NULL, NULL) != SQLITE_OK)
		return set_error(db);

	return 0;
}

static const char *
path(unsigned int idx)
{
	return mlk_util_pathf("%s%u.db", mlk_sys_dir(MLK_SYS_DIR_SAVE), idx);
}

static int
execu(struct save *db, const unsigned char *sql)
{
	return exec(db, (const char *)sql);
}

static int
verify(struct save *db)
{
	struct {
		time_t *date;
		struct property prop;
	} table[] = {
		{ .date = &db->created, { .key = "molko.create-date" } },
		{ .date = &db->updated, { .key = "molko.update-date" } },
	};

	/* Ensure create and update dates are present. */
	for (size_t i = 0; i < MLK_UTIL_SIZE(table); ++i) {
		if (property_load(&table[i].prop, db) < 0) {
			sqlite3_close(db->handle);
			return -1;
		}

		*table[i].date = strtoull(table[i].prop.value, NULL, 10);
	}

	return 0;
}

static int
prepare(struct save *s, struct save_stmt *stmt, const char *sql, const char *args, va_list ap)
{
	stmt->parent = s;
	stmt->handle = NULL;

	if (sqlite3_prepare(s->handle, sql, -1, (sqlite3_stmt **)&stmt->handle, NULL) != SQLITE_OK)
		goto sqlite3_err;

	for (int i = 1; args && *args; ++args) {
		switch (*args) {
		case 'i':
		case 'u':
			if (sqlite3_bind_int(stmt->handle, i++, va_arg(ap, int)) != SQLITE_OK)
				return -1;
			break;
		case 's':
			if (sqlite3_bind_text(stmt->handle, i++, va_arg(ap, const char *), -1, NULL) != SQLITE_OK)
				return -1;
			break;
		case 't':
			if (sqlite3_bind_int64(stmt->handle, i++, va_arg(ap, time_t)) != SQLITE_OK)
				return -1;
			break;
		case ' ':
			break;
		default:
			return set_error(s);
		}
	}

	return 0;

sqlite3_err:
	return set_error(s);
}

static int
extract(struct save_stmt *stmt, const char *args, va_list ap)
{
	const int ncols = sqlite3_column_count(stmt->handle);

	for (int c = 0; args && *args; ++args) {
		if (c >= ncols)
			return set_error(stmt->parent);

		/* TODO: type check. */
		switch (*args) {
		case 'i':
		case 'u':
			*va_arg(ap, int *) = sqlite3_column_int(stmt->handle, c++);
			break;
		case 's': {
			char *str = va_arg(ap, char *);
			size_t max = va_arg(ap, size_t);

			mlk_util_strlcpy(str, (const char *)sqlite3_column_text(stmt->handle, c++), max);
			break;
		}
		case 't':
			*va_arg(ap, time_t *) = sqlite3_column_int64(stmt->handle, c++);
			break;
		case ' ':
			break;
		default:
			return set_error(stmt->parent);
		}
	}

	return 0;
}

int
save_open(struct save *db, unsigned int idx, enum save_mode mode)
{
	assert(db);

	return save_open_path(db, path(idx), mode);
}

int
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
		goto sqlite3_err;

	if (mode == SAVE_MODE_WRITE && execu(db, assets_sql_init) < 0)
		goto sqlite3_err;

	return verify(db);

sqlite3_err:
	set_error(db);
	sqlite3_close(db->handle);

	memset(db, 0, sizeof (*db));

	return -1;
}

int
save_ok(const struct save *db)
{
	assert(db);

	return db && db->handle;
}

int
save_exec(struct save *db, const char *sql, const char *args, ...)
{
	assert(save_ok(db));
	assert(sql);

	struct save_stmt stmt;
	enum save_stmt_errno ret;
	va_list ap;

	va_start(ap, args);
	ret = prepare(db, &stmt, sql, args, ap);
	va_end(ap);

	if (ret < 0)
		return -1;

	ret = save_stmt_next(&stmt, NULL);
	save_stmt_finish(&stmt);

	return ret == SAVE_STMT_ERROR ? -1 : 0;
}

void
save_finish(struct save *db)
{
	assert(db);

	if (db->handle)
		sqlite3_close(db->handle);

	memset(db, 0, sizeof (*db));
}

int
save_stmt_init(struct save_stmt *stmt, struct save *db, const char *sql, const char *args, ...)
{
	assert(stmt);
	assert(save_ok(db));
	assert(args);

	va_list ap;
	int ret;

	va_start(ap, args);
	ret = prepare(db, stmt, sql, args, ap);
	va_end(ap);

	return ret;
}

enum save_stmt_errno
save_stmt_next(struct save_stmt *stmt, const char *args, ...)
{
	assert(stmt);

	va_list ap;
	enum save_stmt_errno ret = SAVE_STMT_ERROR;

	switch (sqlite3_step(stmt->handle)) {
	case SQLITE_ROW:
		va_start(ap, args);

		if (extract(stmt, args, ap) == 0)
			ret = SAVE_STMT_ROW;

		va_end(ap);
		break;
	case SQLITE_DONE:
		ret = SAVE_STMT_DONE;
		break;
	default:
		break;
	}

	return ret;
}

void
save_stmt_finish(struct save_stmt *stmt)
{
	assert(stmt);

	sqlite3_finalize(stmt->handle);
	memset(stmt, 0, sizeof (*stmt));
}

int
save_tx_begin(struct save *s)
{
	assert(save_ok(s));

	return save_exec(s, "BEGIN EXCLUSIVE TRANSACTION", NULL);
}

void
save_tx_rollback(struct save *s)
{
	assert(save_ok(s));

	(void)save_exec(s, "ROLLBACK", NULL);
}

void
save_tx_commit(struct save *s)
{
	assert(save_ok(s));

	(void)save_exec(s, "COMMIT", NULL);
}

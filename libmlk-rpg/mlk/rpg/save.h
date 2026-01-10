/*
 * save.h -- game save files support
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_RPG_SAVE_H
#define MLK_RPG_SAVE_H

/**
 * \file save.h
 * \brief Game save files support
 */

#include <time.h>

#include <mlk/core/core.h>

/**
 * \struct mlk_save
 * \brief Save file structure
 */
struct mlk_save {
	/**
	 * (read-only)
	 *
	 * UTC timestamp when was the file created.
	 */
	time_t created;

	/**
	 * (read-only)
	 *
	 * UTC timestamp when was the last used.
	 */
	time_t updated;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \enum mlk_save_mode
 * \brief Save file access mode
 */
enum mlk_save_mode {
	/**
	 * Open for read-only, fails if the file does not exist.
	 */
	MLK_SAVE_MODE_READ,

	/**
	 * Open for read-write, create the file if not present.
	 */
	MLK_SAVE_MODE_WRITE
};

/**
 * \struct mlk_save_stmt
 * \brief Prepared statement structure
 */
struct mlk_save_stmt {
	/** \cond MLK_PRIVATE_DECLS */
	struct mlk_save *parent;
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

int
mlk_save_open(struct mlk_save *, unsigned int, enum mlk_save_mode);

int
mlk_save_open_path(struct mlk_save *, const char *, enum mlk_save_mode);

int
mlk_save_ok(const struct mlk_save *);

int
mlk_save_exec(struct mlk_save *, const char *, const char *, ...);

void
mlk_save_finish(struct mlk_save *);

/* Prepared statements. */
int
mlk_save_stmt_init(struct mlk_save_stmt *, struct mlk_save *, const char *, const char *, ...);

int
mlk_save_stmt_next(struct mlk_save_stmt *, const char *, ...);

void
mlk_save_stmt_finish(struct mlk_save_stmt *);

/* Explicit transactions. */
int
mlk_save_tx_begin(struct mlk_save *);

int
mlk_save_tx_rollback(struct mlk_save *);

int
mlk_save_tx_commit(struct mlk_save *);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_SAVE_H */

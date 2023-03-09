/*
 * dir.c -- portable directory iterator
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

#include "dir.h"
#include "sysconfig.h"

#if defined(MLK_HAVE_DIRENT_H)

/* {{{ Generic dirent.h support. */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct self {
	DIR *dp;
	struct dirent *entry;
};

static inline int
skip(const struct self *self)
{
	return strcmp(self->entry->d_name, ".")  == 0 ||
	       strcmp(self->entry->d_name, "..") == 0;
}

static void
handle_finish(struct mlk_dir_iter *iter)
{
	struct self *self = iter->handle;

	if (self) {
		closedir(self->dp);
		free(self);
	}

	iter->entry = NULL;
	iter->handle = NULL;
}

static int
handle_open(struct mlk_dir_iter *iter, const char *path)
{
	struct self *self;

	if (!(self = calloc(1, sizeof (*self))))
		return -1;
	if (!(self->dp = opendir(path))) {
		free(self);
		return -1;
	}

	iter->handle = self;

	return 0;
}

static int
handle_next(struct mlk_dir_iter *iter)
{
	struct self *self = iter->handle;

	/* Skip . and .. which are barely useful. */
	while ((self->entry = readdir(self->dp)) && skip(self))
		continue;

	/* End of directory, free all and indicate EOF to the user. */
	if (!self->entry) {
		handle_finish(iter);
		return 0;
	}

	iter->entry = self->entry->d_name;

	return 1;
}

/* }}} */

#else

/* {{{ No-op implementation */

#include <stddef.h>

int
handle_open(struct mlk_dir_iter *iter, const char *path)
{
	(void)path;

	iter->entry = NULL;
	iter->handle = NULL;

	return -1;
}

int
handle_next(struct mlk_dir_iter *iter)
{
	(void)iter;

	return 0;
}

void
handle_finish(struct mlk_dir_iter *iter)
{
	(void)iter;
}

/* }}} */

#endif

int
mlk_dir_open(struct mlk_dir_iter *iter, const char *path)
{
	assert(path);

	return handle_open(iter, path);
}

int
mlk_dir_next(struct mlk_dir_iter *iter)
{
	assert(iter);

	return handle_next(iter);
}

void
mlk_dir_finish(struct mlk_dir_iter *iter)
{
	handle_finish(iter);
}

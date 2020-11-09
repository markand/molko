/*
 * util.c -- utilities
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

#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL.h>

#include "alloc.h"
#include "error.h"
#include "util.h"

void
delay(unsigned int ms)
{
	SDL_Delay(ms);
}

char *
readall(const char *path)
{
	int fd;
	struct stat st;
	char *str = NULL;
	ssize_t nr;

	if ((fd = open(path, O_RDONLY)) < 0 || fstat(fd, &st) < 0)
		goto io_error;
	if (!(str = alloc_zero(1, st.st_size + 1)))
		goto alloc_error;
	if ((nr = read(fd, str, st.st_size)) != st.st_size)
		goto io_error;

	return str;

io_error:
	errorf("%s", strerror(errno));

alloc_error:
	close(fd);
	free(str);

	return NULL;
}

unsigned int
nrand(unsigned int lower, unsigned int upper)
{
	assert(upper <= RAND_MAX);

	return (rand() % (upper - lower + 1)) + lower;
}

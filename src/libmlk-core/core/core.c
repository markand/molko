/*
 * core.c -- libcore main entry
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "sys.h"
#include "translate.h"

int
core_init(const char *organization, const char *name)
{
	assert(organization);
	assert(name);

	srand(time(NULL));

	if (sys_init(organization, name) < 0)
		return -1;

	translate_init("libmlk-core");

	return 0;
}

void
core_finish(void)
{
	sys_finish();
}

/*
 * core.c -- libmlk-core main entry
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

#include "sysconfig.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "core_p.h"
#include "sys.h"

int
mlk_core_init(const char *organization, const char *name)
{
	assert(organization);
	assert(name);

	int err;

	srand(time(NULL));

#if defined(MLK_WITH_NLS)
	bindtextdomain("libmlk-core", mlk_sys_dir(MLK_SYS_DIR_LOCALES));
#endif

	if ((err = mlk_sys_init(organization, name)) < 0)
		return err;

	return 0;
}

void
mlk_core_finish(void)
{
	mlk_sys_finish();
}

/*
 * err.h -- error routines
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

#include <assert.h>
#include <stdio.h>

#include <mlk/util/util.h>

#include "core_p.h"
#include "err.h"

#define ERR_MAX         128
#define DEFAULT_ERR     _("no error")

static MLK_THREAD_LOCAL char err[ERR_MAX];

int
mlk_errf(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	va_start(ap, fmt);
	mlk_errva(fmt, ap);
	va_end(ap);

	return -1;
}

int
mlk_errva(const char *fmt, va_list ap)
{
	assert(fmt);

	int ret;

	/* Don't keep an empty string... */
	ret = vsnprintf(err, sizeof (err), fmt, ap);

	if (ret < 0)
		mlk_util_strlcpy(err, _("unknown error"), sizeof (err));
	else if (ret == 0)
		mlk_util_strlcpy(err, DEFAULT_ERR, sizeof (err));

	return -1;
}

const char *
mlk_err(void)
{
	if (!err[0])
		mlk_util_strlcpy(err, DEFAULT_ERR, sizeof (err));

	return err;
}

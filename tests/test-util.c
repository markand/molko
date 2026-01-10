/*
 * test-util.c -- test utilities
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

#include <mlk/core/util.h>

#include <dt.h>

static void
test_basics_nrand(void)
{
	int found[10] = {0};

	/* Only try from [2..5], mlk_util_nrand does not include upper range.  */
	for (int i = 0; i < 100000; ++i)
		found[mlk_util_nrand(2, 6)] = 1;

	DT_ASSERT(!found[0]);
	DT_ASSERT(!found[1]);
	DT_ASSERT(found[2]);
	DT_ASSERT(found[3]);
	DT_ASSERT(found[4]);
	DT_ASSERT(found[5]);
	DT_ASSERT(!found[7]);
	DT_ASSERT(!found[8]);
	DT_ASSERT(!found[9]);
}

int
main(void)
{
	DT_RUN(test_basics_nrand);
}

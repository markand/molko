/*
 * test-dir.c -- test directory iterator
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

#include <mlk/util/dir.h>

#include <dt.h>

static void
test_basics(void)
{
	struct mlk_dir iter;
	int a = 0, b = 0, c = 0;

	DT_EQ_INT(mlk_dir_open(&iter, DIRECTORY "/dir"), 0);

	/* Order is not guaranteed. */
	while (mlk_dir_next(&iter)) {
		if (strcmp(iter.entry, "a.txt") == 0)
			a = 1;
		if (strcmp(iter.entry, "b.txt") == 0)
			b = 1;
		if (strcmp(iter.entry, "c.txt") == 0)
			c = 1;
	}

	DT_ASSERT(a);
	DT_ASSERT(b);
	DT_ASSERT(c);

	/* At this point, everything should be empty. */
	DT_EQ_PTR(iter.entry, NULL);
	DT_EQ_PTR(iter.handle, NULL);
}

static void
test_break(void)
{
	/*
	 * This test should ensure there are no memory leaks because we break
	 * the loop while the iterator hasn't completed.
	 */
	struct mlk_dir iter;

	DT_EQ_INT(mlk_dir_open(&iter, DIRECTORY "/dir"), 0);
	DT_EQ_INT(mlk_dir_next(&iter), 1);

	mlk_dir_finish(&iter);

	/* At this point, everything should be empty. */
	DT_EQ_PTR(iter.entry, NULL);
	DT_EQ_PTR(iter.handle, NULL);
}

int
main(void)
{
	DT_RUN(test_basics);
	DT_RUN(test_break);
	DT_SUMMARY();
}

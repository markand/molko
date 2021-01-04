/*
 * test-util.c -- test utilities
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

#include <stdbool.h>

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <core/util.h>

GREATEST_TEST
nrand_simple(void)
{
	bool found[10] = {false};

	/* Only try from [2..5], util_nrand does not include upper range. */
	for (int i = 0; i < 100000; ++i)
		found[util_nrand(2, 6)] = true;

	GREATEST_ASSERT_EQ(false, found[0]);
	GREATEST_ASSERT_EQ(false, found[1]);
	GREATEST_ASSERT_EQ(true,  found[2]);
	GREATEST_ASSERT_EQ(true,  found[3]);
	GREATEST_ASSERT_EQ(true,  found[4]);
	GREATEST_ASSERT_EQ(true,  found[5]);
	GREATEST_ASSERT_EQ(false, found[7]);
	GREATEST_ASSERT_EQ(false, found[8]);
	GREATEST_ASSERT_EQ(false, found[9]);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_nrand)
{
	GREATEST_RUN_TEST(nrand_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_nrand);
	GREATEST_MAIN_END();

	return 0;
}

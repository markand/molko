/*
 * test-util.c -- test utilities
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

#include <rexo.h>

#include <core/util.h>

RX_TEST_CASE(nrand, simple)
{
	int found[10] = {0};

	/* Only try from [2..5], util_nrand does not include upper range. */
	for (int i = 0; i < 100000; ++i)
		found[util_nrand(2, 6)] = 1;

	RX_REQUIRE(!found[0]);
	RX_REQUIRE(!found[1]);
	RX_REQUIRE(found[2]);
	RX_REQUIRE(found[3]);
	RX_REQUIRE(found[4]);
	RX_REQUIRE(found[5]);
	RX_REQUIRE(!found[7]);
	RX_REQUIRE(!found[8]);
	RX_REQUIRE(!found[9]);
}

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

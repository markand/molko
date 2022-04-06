/*
 * test-color.c -- test color routines
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

#include <core/color.h>

#include "test.h"

TEST_DECL(basics_red)
{
	RX_INT_REQUIRE_EQUAL(COLOR_R(0xa3000000), 163);
}

TEST_DECL(basics_green)
{
	RX_INT_REQUIRE_EQUAL(COLOR_G(0x00130000), 19);
}

TEST_DECL(basics_blue)
{
	RX_INT_REQUIRE_EQUAL(COLOR_B(0x0000ee00), 238);
}

TEST_DECL(basics_alpha)
{
	RX_INT_REQUIRE_EQUAL(COLOR_A(0x000000ff), 255);
}

TEST_DECL(basics_simple)
{
	RX_UINT_REQUIRE_EQUAL(COLOR_HEX(170, 187, 204, 238), 0xaabbccee);
}

static const struct rx_test_case tests[] = {
	TEST_DEF("basics", "red", basics_red),
	TEST_DEF("basics", "green", basics_green),
	TEST_DEF("basics", "blue", basics_blue),
	TEST_DEF("basics", "alpha", basics_alpha),
	TEST_DEF("basics", "simple", basics_simple)
};

int
main(int argc, char **argv)
{
	return TEST_RUN(tests, argc, argv);
}

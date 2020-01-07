/*
 * test-color.c -- test color routines
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

#include <greatest.h>

#include <color.h>

TEST
red(void)
{
	ASSERT_EQ(163, COLOR_R(0xa3000000));
	PASS();
}

TEST
green(void)
{
	ASSERT_EQ(19, COLOR_G(0x00130000));
	PASS();
}

TEST
blue(void)
{
	ASSERT_EQ(238, COLOR_B(0x0000ee00));
	PASS();
}

TEST
alpha(void)
{
	ASSERT_EQ(255, COLOR_A(0x000000ff));
	PASS();
}

TEST
simple(void)
{
	ASSERT_EQ(0xaabbccee, COLOR_HEX(170, 187, 204, 238));
	PASS();
}

SUITE(hex_to_rgb)
{
	RUN_TEST(red);
	RUN_TEST(green);
	RUN_TEST(blue);
	RUN_TEST(alpha);
}

SUITE(rgb_to_hex)
{
	RUN_TEST(simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(hex_to_rgb);
	RUN_SUITE(rgb_to_hex);
	GREATEST_MAIN_END();
}

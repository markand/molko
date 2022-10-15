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

#include <mlk/core/color.h>

#include <dt.h>

static void
test_basics_red(void)
{
	DT_EQ_INT(COLOR_R(0xa3000000), 163);
}

static void
test_basics_green(void)
{
	DT_EQ_INT(COLOR_G(0x00130000), 19);
}

static void
test_basics_blue(void)
{
	DT_EQ_INT(COLOR_B(0x0000ee00), 238);
}

static void
test_basics_alpha(void)
{
	DT_EQ_INT(COLOR_A(0x000000ff), 255);
}

static void
test_basics_simple(void)
{
	DT_EQ_UINT(COLOR_HEX(170, 187, 204, 238), 0xaabbccee);
}

int
main(void)
{
	DT_RUN(test_basics_red);
	DT_RUN(test_basics_green);
	DT_RUN(test_basics_blue);
	DT_RUN(test_basics_alpha);
	DT_RUN(test_basics_simple);
	DT_SUMMARY();
}

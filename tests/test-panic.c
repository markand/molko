/*
 * test-panic.c -- test panic routines
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

#include <error.h>
#include <panic.h>

static bool handler_called;

static void
handler(void)
{
	handler_called = true;
}

GREATEST_TEST
basics_simple(void)
{
	panic_handler = handler;
	handler_called = false;

	panic("this is an error");
	GREATEST_ASSERT(handler_called);
	GREATEST_ASSERT_STR_EQ(error(), "this is an error");
	GREATEST_PASS();
}

GREATEST_SUITE(basics)
{
	GREATEST_RUN_TEST(basics_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(basics);
	GREATEST_MAIN_END();
}

/*
 * test-save.c -- test save routines
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

#include <stdio.h>

#include <greatest.h>

#include "save.h"

#define NAME "test.db"

static void
clean(void *data)
{
	(void)data;

	save_finish();
	remove(NAME);
}

TEST
properties_simple(void)
{
	ASSERT(save_open_path(NAME));

	/* Insert a new property 'initialized'. */
	ASSERT(save_set_property("initialized", "1"));
	ASSERT_STR_EQ(save_get_property("initialized"), "1");

	/* This must replace the existing value. */
	ASSERT(save_set_property("initialized", "2"));
	ASSERT_STR_EQ(save_get_property("initialized"), "2");

	ASSERT(save_remove_property("initialized"));
	ASSERT(!save_get_property("initialized"));

	PASS();
}

SUITE(properties)
{
	SET_TEARDOWN(clean, NULL);
	RUN_TEST(properties_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(properties);
	GREATEST_MAIN_END();
}

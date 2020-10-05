/*
 * test-rbuf.c -- test error routines
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

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <rbuf.h>

/* Don't use "" to avoid '\0' */
const char data1[] = { 'a', 'b', 'c', '\n', 'd', 'e', 'f', '\n' };
const char data2[] = { 'a', 'b', 'c', '\n', 'd', 'e', 'f' };
const char data3[] = { 'a', 'b', 'c', '\n', '\n', 'd', 'e', 'f', '\n' };
const char data4[] = { 'a', 'b', 'c', '\n', 'd', 'e', 'f', 'g', 'h', '\n' };

GREATEST_TEST
simple(void)
{
	struct rbuf rb;
	char line[128];

	rbuf_open(&rb, data1, sizeof (data1));

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("abc", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("def", line);

	GREATEST_PASS();
}

GREATEST_TEST
simple_noeol(void)
{
	struct rbuf rb;
	char line[128];

	rbuf_open(&rb, data2, sizeof (data2));

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("abc", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("def", line);

	GREATEST_PASS();
}

GREATEST_TEST
simple_emptylines(void)
{
	struct rbuf rb;
	char line[128];

	rbuf_open(&rb, data3, sizeof (data3));

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("abc", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("def", line);

	GREATEST_PASS();
}

GREATEST_SUITE(simple_suite)
{
	GREATEST_RUN_TEST(simple);
	GREATEST_RUN_TEST(simple_noeol);
	GREATEST_RUN_TEST(simple_emptylines);
}

GREATEST_TEST
fail_toosmall(void)
{
	struct rbuf rb;
	char line[4];

	rbuf_open(&rb, data4, sizeof (data4));

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("abc", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(!rbuf_readline(&rb, line, sizeof (line)));

	GREATEST_PASS();
}

GREATEST_TEST
fail_end(void)
{
	struct rbuf rb;
	char line[128];

	rbuf_open(&rb, data1, sizeof (data1));

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("abc", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(rbuf_readline(&rb, line, sizeof (line)));
	GREATEST_ASSERT_STR_EQ("def", line);

	memset(line, '?', sizeof (line));
	GREATEST_ASSERT(!rbuf_readline(&rb, line, sizeof (line)));

	GREATEST_PASS();
}

GREATEST_SUITE(fail_suite)
{
	GREATEST_RUN_TEST(fail_toosmall);
	GREATEST_RUN_TEST(fail_end);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(simple_suite);
	GREATEST_RUN_SUITE(fail_suite);
	GREATEST_MAIN_END();
}

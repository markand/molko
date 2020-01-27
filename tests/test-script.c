/*
 * test-script.c -- test script action
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

#include <script.h>

static bool
truth(struct action *a, unsigned int ticks)
{
	(void)a;
	(void)ticks;

	return true;
}

static bool
nope(struct action *a, unsigned int ticks)
{
	(void)a;
	(void)ticks;

	return false;
}

TEST
append(void)
{
	int ref1 = 0, ref2 = 0;
	struct script sc;

	script_init(&sc);

	/* ref1 */
	script_append(&sc, &(struct action) {
		.update = truth,
		.data = &ref1
	});

	ASSERT(sc.head);
	ASSERT_EQ(sc.head->action.data, &ref1);

	/* ref2 */
	script_append(&sc, &(struct action) {
		.update = truth,
		.data = &ref2
	});

	ASSERT(sc.head);
	ASSERT_EQ(sc.head->action.data, &ref1);
	ASSERT_EQ(sc.head->next->action.data, &ref2);

	script_finish(&sc);

	PASS();
}

TEST
finish(void)
{
	struct script sc;

	script_init(&sc);
	script_append(&sc, &(struct action) {
		.update = truth,
	});
	script_append(&sc, &(struct action) {
		.update = truth,
	});
	script_finish(&sc);

	ASSERT(!sc.head);
	PASS();
}

SUITE(manipulation)
{
	RUN_TEST(append);
	RUN_TEST(finish);
}

TEST
advance(void)
{
	int ref1 = 0, ref2 = 0;
	struct script sc;

	script_init(&sc);

	script_append(&sc, &(struct action) {
		.update = truth,
		.data = &ref1
	});
	script_append(&sc, &(struct action) {
		.update = truth,
		.data = &ref2
	});

	/* Start -> 1 */
	script_start(&sc);

	ASSERT(sc.iter);
	ASSERT_EQ(sc.iter->action.data, &ref1);

	/* Update -> 2 */
	script_update(&sc, 0);

	ASSERT(sc.iter);
	ASSERT_EQ(sc.iter->action.data, &ref2);

	/* Update -> end */
	script_update(&sc, 0);

	ASSERT(!sc.iter);

	script_finish(&sc);

	PASS();
}

TEST
dont_advance(void)
{
	int ref1 = 0, ref2 = 0;
	struct script sc;

	script_init(&sc);

	script_append(&sc, &(struct action) {
		.update = nope,
		.data = &ref1
	});
	script_append(&sc, &(struct action) {
		.update = truth,
		.data = &ref2
	});

	/* Start -> 1 */
	script_start(&sc);

	ASSERT(sc.iter);
	ASSERT_EQ(sc.iter->action.data, &ref1);

	/* Update -> 1 */
	script_update(&sc, 0);

	ASSERT(sc.iter);
	ASSERT_EQ(sc.iter->action.data, &ref1);

	script_finish(&sc);

	PASS();
}

SUITE(usage)
{
	RUN_TEST(advance);
	RUN_TEST(dont_advance);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(manipulation);
	RUN_SUITE(usage);
	GREATEST_MAIN_END();
}

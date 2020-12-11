/*
 * test-alloc.c -- test allocators
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

#include <core/alloc.h>

struct point {
	int x;
	int y;
};

GREATEST_TEST
test_array_simple(void)
{
	struct point *points;

	GREATEST_ASSERT((points = alloc_array0(2, sizeof (*points))));
	GREATEST_ASSERT_EQ(0, points[0].x);
	GREATEST_ASSERT_EQ(0, points[0].y);
	GREATEST_ASSERT_EQ(0, points[1].x);
	GREATEST_ASSERT_EQ(0, points[1].y);

	points[0].x = 10;
	points[0].y = 20;
	points[1].x = 30;
	points[1].y = 40;

	GREATEST_ASSERT((points = alloc_rearray0(points, 2, 4, sizeof (*points))));
	GREATEST_ASSERT_EQ(10, points[0].x);
	GREATEST_ASSERT_EQ(20, points[0].y);
	GREATEST_ASSERT_EQ(30, points[1].x);
	GREATEST_ASSERT_EQ(40, points[1].y);
	GREATEST_ASSERT_EQ(0,  points[2].x);
	GREATEST_ASSERT_EQ(0,  points[2].y);
	GREATEST_ASSERT_EQ(0,  points[3].x);
	GREATEST_ASSERT_EQ(0,  points[3].y);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_RUN_TEST(test_array_simple);
}

GREATEST_TEST
test_pool_simple(void)
{
	struct alloc_pool pool;
	struct point *p;

	GREATEST_ASSERT(alloc_pool_init(&pool, sizeof (*p), NULL));
	GREATEST_ASSERT_EQ(sizeof (*p), pool.elemsize);
	GREATEST_ASSERT_EQ(0, pool.size);
	GREATEST_ASSERT_EQ(ALLOC_POOL_INIT_DEFAULT, pool.capacity);

	/* Create until we reach the capacity. */
	for (size_t i = 0; i < pool.capacity; ++i) {
		p = alloc_pool_new(&pool);
		p->x = (int)i + 1;
		p->y = (int)i + 1;
	}

	GREATEST_ASSERT_EQ(pool.size, pool.capacity);

	/* Verify values are correct. */
	for (size_t i = 0; i < pool.size; ++i) {
		p = ((struct point *)pool.data) + i;

		GREATEST_ASSERT_EQ((int)i + 1, p->x);
		GREATEST_ASSERT_EQ((int)i + 1, p->y);
	}

	/* Now it should reallocate. */
	p = alloc_pool_new(&pool);
	p->x = 9999;
	p->y = 9999;

	GREATEST_ASSERT(pool.capacity > pool.size);

	alloc_pool_finish(&pool);

	GREATEST_ASSERT_EQ(NULL, pool.data);
	GREATEST_ASSERT_EQ(0, pool.elemsize);
	GREATEST_ASSERT_EQ(0, pool.size);
	GREATEST_ASSERT_EQ(0, pool.capacity);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_pool)
{
	GREATEST_RUN_TEST(test_pool_simple);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_RUN_SUITE(suite_pool);
	GREATEST_MAIN_END();

	return 0;
}

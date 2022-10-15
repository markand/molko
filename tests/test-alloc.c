/*
 * test-alloc.c -- test allocators
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

#include <stdlib.h>

#include <mlk/core/alloc.h>

#include <dt.h>

struct point {
	int x;
	int y;
};

static struct {
	size_t total;
	size_t alloc_count;
	size_t free_count;
} my_stats;

static void *
my_alloc(size_t n)
{
	my_stats.alloc_count += 1;
	my_stats.total += n;

	return malloc(n);
}

static void *
my_realloc(void *ptr, size_t n)
{
	(void)ptr;
	(void)n;

	return NULL;
}

static void
my_free(void *ptr)
{
	free(ptr);
	my_stats.free_count += 1;
}

static const struct alloc_funcs my_funcs = {
	.alloc = my_alloc,
	.realloc = my_realloc,
	.free = my_free
};

static void
test_basics_array_simple(void)
{
	struct point *points;

	DT_ASSERT((points = alloc_array0(2, sizeof (*points))));
	DT_EQ_INT(points[0].x, 0);
	DT_EQ_INT(points[0].y, 0);
	DT_EQ_INT(points[1].x, 0);
	DT_EQ_INT(points[1].y, 0);

	points[0].x = 10;
	points[0].y = 20;
	points[1].x = 30;
	points[1].y = 40;

	DT_ASSERT((points = alloc_rearray0(points, 2, 4, sizeof (*points))));
	DT_EQ_INT(points[0].x, 10);
	DT_EQ_INT(points[0].y, 20);
	DT_EQ_INT(points[1].x, 30);
	DT_EQ_INT(points[1].y, 40);
	DT_EQ_INT(points[2].x, 0);
	DT_EQ_INT(points[2].y, 0);
	DT_EQ_INT(points[3].x, 0);
	DT_EQ_INT(points[3].y, 0);
}

static void
test_basics_pool_simple(void)
{
	struct alloc_pool pool;
	struct point *p, *data;
	size_t total = 0;

	alloc_pool_init(&pool, sizeof (*p), NULL);

	DT_EQ_UINT(pool.elemsize, sizeof (*p));
	DT_EQ_UINT(pool.size, 0);
	DT_EQ_UINT(pool.capacity, ALLOC_POOL_INIT_DEFAULT);

	/* Create until we reach the capacity. */
	for (size_t i = 0; i < pool.capacity; ++i) {
		p = alloc_pool_new(&pool);
		p->x = (int)i + 1;
		p->y = (int)i + 1;
		total++;
	}

	DT_EQ_UINT(pool.size, pool.capacity);

	/* Verify values are correct. */
	for (size_t i = 0; i < pool.size; ++i) {
		p = ((struct point *)pool.data) + i;

		DT_EQ_INT(p->x, (int)i + 1);
		DT_EQ_INT(p->y, (int)i + 1);
	}

	/* Now it should reallocate. */
	p = alloc_pool_new(&pool);
	p->x = 9999;
	p->y = 9999;

	DT_ASSERT(pool.capacity > pool.size);

	/* Shrink it! */
	data = alloc_pool_shrink(&pool);

	/* Verify values are correct again. */
	for (size_t i = 0; i < total; ++i) {
		DT_EQ_INT(data[i].x, (int)i + 1);
		DT_EQ_INT(data[i].y, (int)i + 1);
	}

	DT_EQ_PTR(pool.data, NULL);
	DT_EQ_UINT(pool.size, 0U);
	DT_EQ_UINT(pool.capacity, 0U);
	DT_EQ_UINT(pool.elemsize, 0U);
}

static void
test_basics_sdupf(void)
{
	char *str = alloc_sdupf("Hello %s", "David");

	DT_EQ_STR(str, "Hello David");
	free(str);
}

static void
test_custom_count(void)
{
	alloc_set(&my_funcs);
	alloc_free(alloc_new(10));
	alloc_free(alloc_new0(20));
	alloc_free(alloc_sdup("malikania"));

	DT_EQ_UINT(my_stats.total, 40U);
	DT_EQ_UINT(my_stats.alloc_count, 3U);
	DT_EQ_UINT(my_stats.free_count, 3U);
}

int
main(void)
{
	DT_RUN(test_basics_array_simple);
	DT_RUN(test_basics_pool_simple);
	DT_RUN(test_basics_sdupf);
	DT_RUN(test_custom_count);
	DT_SUMMARY();
}

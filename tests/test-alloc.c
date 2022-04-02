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

#include <rexo.h>

#include <core/alloc.h>

struct point {
	int x;
	int y;
};

static const struct alloc_funcs standard_funcs = {
	.alloc = malloc,
	.realloc = realloc,
	.free = free
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

RX_SET_UP(standard_setup)
{
	alloc_set(&standard_funcs);

	return RX_SUCCESS;
}

RX_SET_UP(my_setup)
{
	alloc_set(&my_funcs);

	return RX_SUCCESS;
}

RX_FIXTURE(standard_fixture, void *, .set_up = standard_setup);

RX_TEST_CASE(test, array_simple, .fixture = standard_fixture)
{
	struct point *points;

	RX_REQUIRE((points = alloc_array0(2, sizeof (*points))));
	RX_INT_REQUIRE_EQUAL(points[0].x, 0);
	RX_INT_REQUIRE_EQUAL(points[0].y, 0);
	RX_INT_REQUIRE_EQUAL(points[1].x, 0);
	RX_INT_REQUIRE_EQUAL(points[1].y, 0);

	points[0].x = 10;
	points[0].y = 20;
	points[1].x = 30;
	points[1].y = 40;

	RX_REQUIRE((points = alloc_rearray0(points, 2, 4, sizeof (*points))));
	RX_INT_REQUIRE_EQUAL(points[0].x, 10);
	RX_INT_REQUIRE_EQUAL(points[0].y, 20);
	RX_INT_REQUIRE_EQUAL(points[1].x, 30);
	RX_INT_REQUIRE_EQUAL(points[1].y, 40);
	RX_INT_REQUIRE_EQUAL(points[2].x, 0);
	RX_INT_REQUIRE_EQUAL(points[2].y, 0);
	RX_INT_REQUIRE_EQUAL(points[3].x, 0);
	RX_INT_REQUIRE_EQUAL(points[3].y, 0);
}

RX_TEST_CASE(test, pool_simple, .fixture = standard_fixture)
{
	struct alloc_pool pool;
	struct point *p, *data;
	size_t total = 0;

	alloc_pool_init(&pool, sizeof (*p), NULL);

	RX_UINT_REQUIRE_EQUAL(pool.elemsize, sizeof (*p));
	RX_UINT_REQUIRE_EQUAL(pool.size, 0);
	RX_UINT_REQUIRE_EQUAL(pool.capacity, ALLOC_POOL_INIT_DEFAULT);

	/* Create until we reach the capacity. */
	for (size_t i = 0; i < pool.capacity; ++i) {
		p = alloc_pool_new(&pool);
		p->x = (int)i + 1;
		p->y = (int)i + 1;
		total++;
	}

	RX_UINT_REQUIRE_EQUAL(pool.size, pool.capacity);

	/* Verify values are correct. */
	for (size_t i = 0; i < pool.size; ++i) {
		p = ((struct point *)pool.data) + i;

		RX_INT_REQUIRE_EQUAL(p->x, (int)i + 1);
		RX_INT_REQUIRE_EQUAL(p->y, (int)i + 1);
	}

	/* Now it should reallocate. */
	p = alloc_pool_new(&pool);
	p->x = 9999;
	p->y = 9999;

	RX_REQUIRE(pool.capacity > pool.size);

	/* Shrink it! */
	data = alloc_pool_shrink(&pool);

	/* Verify values are correct again. */
	for (size_t i = 0; i < total; ++i) {
		RX_INT_REQUIRE_EQUAL(data[i].x, (int)i + 1);
		RX_INT_REQUIRE_EQUAL(data[i].y, (int)i + 1);
	}

	RX_PTR_REQUIRE_EQUAL(pool.data, NULL);
	RX_UINT_REQUIRE_EQUAL(pool.size, 0);
	RX_UINT_REQUIRE_EQUAL(pool.capacity, 0);
	RX_UINT_REQUIRE_EQUAL(pool.elemsize, 0);
}

RX_TEST_CASE(test, sdupf, .fixture = standard_fixture)
{
	char *str = alloc_sdupf("Hello %s", "David");

	RX_STR_REQUIRE_EQUAL(str, "Hello David");
	free(str);
}

RX_FIXTURE(my_fixture, void *, .set_up = my_setup);

RX_TEST_CASE(test, custom, .fixture = my_fixture)
{
	alloc_free(alloc_new(10));
	alloc_free(alloc_new0(20));
	alloc_free(alloc_sdup("malikania"));

	RX_INT_REQUIRE_EQUAL(my_stats.total, 40U);
	RX_INT_REQUIRE_EQUAL(my_stats.alloc_count, 3U);
	RX_INT_REQUIRE_EQUAL(my_stats.free_count, 3U);
}

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}

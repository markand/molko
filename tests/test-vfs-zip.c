/*
 * test-zip-directory.c -- test VFS zip
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

#include <core/vfs-zip.h>
#include <core/vfs.h>

#include "test.h"

RX_SET_UP(basics_set_up)
{
	if (vfs_zip(RX_DATA, DIRECTORY "/vfs/data.zip", "r") < 0)
		return RX_ERROR;

	return RX_SUCCESS;
}

RX_TEAR_DOWN(basics_tear_down)
{
	vfs_finish(RX_DATA);
}

#define error_set_up basics_set_up
#define error_tear_down basics_tear_down

RX_TEST_CASE(basics, read)
{
	struct vfs_file file;
	char data[256] = {0};

	RX_INT_REQUIRE_EQUAL(vfs_open(RX_DATA, &file, "texts/hello.txt", "r"), 0);
	RX_UINT_REQUIRE_EQUAL(vfs_file_read(&file, data, sizeof (data)), 21U);
	RX_STR_REQUIRE_EQUAL(data, "Hello from zip file!\n");
}

RX_TEST_CASE(error, notfound)
{
	struct vfs_file file;

	RX_INT_REQUIRE_EQUAL(vfs_open(RX_DATA, &file, "notfound.txt", "r"), -1);
}

static const struct rx_test_case tests[] = {
	TEST_FIXTURE(basics, read, struct vfs),
	TEST_FIXTURE(error, notfound, struct vfs)
};

int
main(int argc, char **argv)
{
	return TEST_RUN_ALL(tests, argc, argv);
}

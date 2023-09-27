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

#include <mlk/core/vfs-zip.h>

#include <dt.h>

static void
test_basics_read(void)
{
	struct mlk_vfs_zip zip;
	struct mlk_vfs_file *file;
	char data[256] = {};

	DT_EQ_INT(mlk_vfs_zip_init(&zip, DIRECTORY "/vfs/data.zip", "r"), 0);
	DT_ASSERT(file = mlk_vfs_open(&zip.vfs, "texts/hello.txt", "r"));
	DT_EQ_UINT(mlk_vfs_file_read(file, data, sizeof (data)), 21U);
	DT_EQ_STR(data, "Hello from zip file!\n");

	mlk_vfs_file_free(file);
	mlk_vfs_finish(&zip.vfs);
}

static void
test_error_notfound(void)
{
	struct mlk_vfs_zip zip;

	DT_EQ_INT(mlk_vfs_zip_init(&zip, DIRECTORY "/vfs/data.zip", "r"), 0);
	DT_ASSERT(!mlk_vfs_open(&zip.vfs, "notfound.txt", "r"));

	mlk_vfs_finish(&zip.vfs);
}

int
main(void)
{
	DT_RUN(test_basics_read);
	DT_RUN(test_error_notfound);
	DT_SUMMARY();
}

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

#include <dt.h>

static void
test_basics_read(void)
{
	struct vfs vfs;
	struct vfs_file file;
	char data[256] = {0};

	vfs_zip(&vfs, DIRECTORY "/vfs/data.zip", "r");

	DT_EQ_INT(vfs_open(&vfs, &file, "texts/hello.txt", "r"), 0);
	DT_EQ_UINT(vfs_file_read(&file, data, sizeof (data)), 21U);
	DT_EQ_STR(data, "Hello from zip file!\n");

	vfs_finish(&vfs);
}

static void
test_error_notfound(void)
{
	struct vfs vfs;
	struct vfs_file file;

	vfs_zip(&vfs, DIRECTORY "/vfs/data.zip", "r");

	DT_EQ_INT(vfs_open(&vfs, &file, "notfound.txt", "r"), -1);

	vfs_finish(&vfs);
}

int
main(void)
{
	DT_RUN(test_basics_read);
	DT_RUN(test_error_notfound);
	DT_SUMMARY();
}

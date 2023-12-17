/*
 * main.c -- main mlk-run file
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
#include <stdlib.h>
#include <string.h>

#include <duktape.h>
#include <duk_module.h>

#include <mlk/util/util.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/panic.h>
#include <mlk/core/vfs-dir.h>
#include <mlk/core/vfs-zip.h>
#include <mlk/core/vfs.h>

#include <mlk/core/js/js.h>

union {
	struct mlk_vfs_dir dir;

#if defined(MLK_WITH_ZIP)
	struct mlk_vfs_zip zip;
#endif
} iface;

/* VFS loader to support zip and directories when loading game. */
static struct mlk_vfs *vfs;

/* Global Javascript context. */
static struct mlk_js js;

static void
init(void)
{
	/* TODO: this is temporary. */
	if (mlk_core_init("fr.malikania", "mlk-run") < 0)
		mlk_panic();
}

static void
load_zip(const char *path)
{
#if defined(MLK_WITH_ZIP)
	if (mlk_vfs_zip_init(&iface.zip, path, "r") < 0)
		mlk_panic();

	vfs = &iface.zip.vfs;
#else
	mlk_panicf("ZIP file support disabled")a;
#endif
}

static void
load_directory(const char *path)
{
	mlk_vfs_dir_init(&iface.dir, path);
	vfs = &iface.dir.vfs;
}

static void
load(const char *path)
{
	/* TODO: improve this. */
	if (strstr(path, ".mlk") || strstr(path, ".zip"))
		load_zip(path);
	else
		load_directory(path);

	if (mlk_js_init(&js, vfs) < 0 || mlk_js_run(&js, MLK_CORE_JS_MAIN))
		mlk_panic();
}

static void
finish(void)
{
	mlk_js_finish(&js);
	mlk_vfs_finish(vfs);
}

static void
run(int argc, char **argv)
{
	if (argc < 0) {
		fprintf(stderr, "usage: mlk-run game\n");
		exit(1);
	}

	load(argv[0]);
}

int
main(int argc, char **argv)
{
	--argc;
	++argv;

	init();
	run(argc, argv);
	finish();
}

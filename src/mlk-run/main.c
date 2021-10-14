/*
 * main.c -- main mlk-run file
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <duktape.h>

#include <core/panic.h>
#include <core/vfs-directory.h>
#include <core/vfs-zip.h>
#include <core/vfs.h>

#include <core/js-window.h>

/* VFS loader to support zip and directories when loading game. */
static struct vfs vfs;

/* Javascript context. */
static duk_context *ctx;

static void
core_bind(duk_context *ctx)
{
	js_window_bind(ctx);
}

static void
init(void)
{
	/* TODO: this is temporary. */
	if (core_init("fr.malikania", "mlk-run") < 0)
		panic();

	/* Fireup Javascript. */
	ctx = duk_create_heap_default();
	core_bind(ctx);
}

static char *
extract(struct vfs_file *file)
{
	FILE *fp;
	char *out, buf[BUFSIZ];
	size_t len, nr;

	if (!(fp = open_memstream(&out, &len)))
		panic();

	while ((nr = vfs_file_read(file, buf, sizeof (buf))) > 0)
		if (fwrite(buf, 1, nr, fp) <= 0)
			panic();

	fclose(fp);

	return out;
}

static void
startup(void)
{
	struct vfs_file main;
	char *code;

	if (vfs_open(&vfs, &main, "main.js", "r") < 0)
		panic();

	code = extract(&main);
	vfs_file_finish(&main);

	if (duk_peval_string(ctx, code))
		panicf("%s", duk_safe_to_string(ctx, -1));

	free(code);
}

static void
finish(void)
{
	vfs_finish(&vfs);
	duk_destroy_heap(ctx);
}

static void
loadzip(const char *path)
{
	if (vfs_zip(&vfs, path, "r") < 0)
		panic();
}

static void
loaddirectory(const char *path)
{
	vfs_directory(&vfs, path);
}

static void
load(const char *path)
{
	/* TODO: improve this. */
	if (strstr(path, ".mlk"))
		loadzip(path);
	else
		loaddirectory(path);

	startup();
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

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
#include <limits.h>

#include <duktape.h>
#include <duk_module.h>

#include <mlk/util/util.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/panic.h>
#include <mlk/core/vfs-dir.h>
#include <mlk/core/vfs-zip.h>
#include <mlk/core/vfs.h>

union {
	struct mlk_vfs_dir dir;

#if defined(MLK_WITH_ZIP)
	struct mlk_vfs_zip zip;
#endif
} iface;

/* VFS loader to support zip and directories when loading game. */
static struct mlk_vfs *vfs;

/* Javascript context. */
static duk_context *ctx;

static duk_ret_t
modsearch(duk_context *ctx)
{
	char path[MLK_PATH_MAX] = {}, *data;
	struct mlk_vfs_file *file;
	size_t datasz;

	snprintf(path, sizeof (path), "%s.js", duk_require_string(ctx, 0));

	if ((file = mlk_vfs_open(vfs, path, "r")) < 0)
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());
	if (!(data = mlk_vfs_file_read_all(file, &datasz))) {
		mlk_vfs_file_finish(file);
		return duk_error(ctx, DUK_ERR_ERROR, "%s", mlk_err());
	}

	mlk_vfs_file_finish(file);
	duk_push_lstring(ctx, data, datasz);
	free(data);

	return 1;
}

static void
core_bind(duk_context *ctx)
{
#if 0
	/* Brings Mlk global object. */
	mlk_js_core_bind(ctx, &vfs);

	js_action_bind(ctx);
	js_action_stack_bind(ctx);
	js_animation_bind(ctx);
	js_clock_bind(ctx);
	js_color_bind(ctx);
	js_drawable_bind(ctx);
	js_drawable_stack_bind(ctx);
	js_event_bind(ctx);
	js_font_bind(ctx);
	js_game_bind(ctx);
	js_music_bind(ctx);
	js_painter_bind(ctx);
	js_panic_bind(ctx);
	js_sound_bind(ctx);
	js_sprite_bind(ctx);
	js_state_bind(ctx);
	js_texture_bind(ctx);
	js_trace_bind(ctx);
	js_window_bind(ctx);
#endif
}

static void
init(void)
{
	/* TODO: this is temporary. */
	if (mlk_core_init("fr.malikania", "mlk-run") < 0)
		mlk_panic();

	/* Fireup Javascript. */
	ctx = duk_create_heap_default();
	core_bind(ctx);

	/* Setup module loader. */
	duk_module_duktape_init(ctx);
	duk_get_global_string(ctx, "Duktape");
	duk_push_c_function(ctx, modsearch, 4);
	duk_put_prop_string(ctx, -2, "modSearch");
	duk_pop(ctx);
}

static void
startup(void)
{
	struct mlk_vfs_file *file;
	char *code;

	if (!(file = mlk_vfs_open(vfs, "main.js", "r")))
		mlk_panic();
	if (!(code = mlk_vfs_file_read_all(file, NULL))) {
		mlk_vfs_file_finish(file);
		mlk_panic();
	}

	mlk_vfs_file_finish(file);

	if (duk_peval_string(ctx, code))
		mlk_panicf("%s", duk_safe_to_string(ctx, -1));

	free(code);
}

static void
finish(void)
{
	mlk_vfs_finish(vfs);
	duk_destroy_heap(ctx);
}

static void
loadzip(const char *path)
{
#if 0
	if (vfs_zip(&vfs, path, "r") < 0)
		panic();
#endif
}

static void
loaddirectory(const char *path)
{
#if 0
	vfs_directory(&vfs, path);
#endif
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

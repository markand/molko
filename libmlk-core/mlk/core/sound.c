/*
 * sound.c -- sound support
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "sound.h"
#include "sys_p.h"
#include "vfs.h"

#define SOURCE(snd) ((const struct mlk__audiostream *)snd->handle)->source

int
mlk_sound_open(struct mlk_sound *snd, const char *path)
{
	assert(snd);
	assert(path);

	return mlk__audiostream_open((struct mlk__audiostream **)&snd->handle, path);
}

int
mlk_sound_openmem(struct mlk_sound *snd, const void *buffer, size_t buffersz)
{
	assert(snd);
	assert(buffer);

	return mlk__audiostream_openmem((struct mlk__audiostream **)&snd->handle, buffer, buffersz);
}

int
mlk_sound_openvfs(struct mlk_sound *snd, struct mlk_vfs_file *file)
{
	assert(snd);
	assert(file);

	char *data;
	size_t datasz;
	int ret = 0;

	if (!(data = mlk_vfs_file_read_all(file, &datasz)))
		return -1;
	if (mlk__audiostream_openmem((struct mlk__audiostream **)&snd->handle, data, datasz) < 0)
		ret = -1;

	mlk_alloc_free(data);

	return ret;
}

int
mlk_sound_play(struct mlk_sound *snd)
{
	assert(snd);

	alSourcePlay(SOURCE(snd));

	return 0;
}

void
mlk_sound_pause(struct mlk_sound *snd)
{
	assert(snd);

	alSourcePause(SOURCE(snd));
}

void
mlk_sound_resume(struct mlk_sound *snd)
{
	assert(snd);

	alSourcePlay(SOURCE(snd));
}

void
mlk_sound_stop(struct mlk_sound *snd)
{
	assert(snd);

	alSourceStop(SOURCE(snd));
}

void
mlk_sound_finish(struct mlk_sound *snd)
{
	assert(snd);

	if (snd->handle) {
		mlk_sound_stop(snd);
		mlk__audiostream_finish(snd->handle);
	}

	memset(snd, 0, sizeof (*snd));
}

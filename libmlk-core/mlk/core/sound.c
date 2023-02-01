/*
 * sound.c -- sound support
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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "sound.h"
#include "sys_p.h"

#define SOURCE(snd) ((const struct audiostream *)snd->handle)->source

int
sound_open(struct sound *snd, const char *path)
{
	assert(snd);
	assert(path);

	return audiostream_open((struct audiostream **)&snd->handle, path);
}

int
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz)
{
	assert(snd);
	assert(buffer);

	return audiostream_openmem((struct audiostream **)&snd->handle, buffer, buffersz);
}

int
sound_ok(const struct sound *snd)
{
	return snd && snd->handle;
}

int
sound_play(struct sound *snd)
{
	assert(sound_ok(snd));

	alSourcePlay(SOURCE(snd));

	return 0;
}

void
sound_pause(struct sound *snd)
{
	assert(sound_ok(snd));

	alSourcePause(SOURCE(snd));
}

void
sound_resume(struct sound *snd)
{
	assert(sound_ok(snd));

	alSourcePlay(SOURCE(snd));
}

void
sound_stop(struct sound *snd)
{
	assert(sound_ok(snd));

	alSourceStop(SOURCE(snd));
}

void
sound_finish(struct sound *snd)
{
	assert(snd);

	if (snd->handle) {
		sound_stop(snd);
		audiostream_finish(snd->handle);
	}

	memset(snd, 0, sizeof (*snd));
}

/*
 * music.h -- music support
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

#include <assert.h>
#include <string.h>

#include "err.h"
#include "music.h"
#include "sys_p.h"

#define SOURCE(mus) ((const struct audiostream *)mus->handle)->source

int
music_open(struct music *mus, const char *path)
{
	assert(mus);
	assert(path);

	return audiostream_open((struct audiostream **)&mus->handle, path);
}

int
music_openmem(struct music *mus, const void *buffer, size_t buffersz)
{
	assert(mus);
	assert(buffer);

	return audiostream_openmem((struct audiostream **)&mus->handle, buffer, buffersz);
}

int
music_ok(const struct music *mus)
{
	return mus && mus->handle;
}

int
music_play(struct music *mus, enum music_flags flags)
{
	assert(mus);

	if (flags & MUSIC_LOOP)
		alSourcei(SOURCE(mus), AL_LOOPING, AL_TRUE);
	else
		alSourcei(SOURCE(mus), AL_LOOPING, AL_TRUE);

	alSourceRewind(SOURCE(mus));
	alSourcePlay(SOURCE(mus));

	return 0;
}

void
music_pause(struct music *mus)
{
	assert(music_ok(mus));

	alSourcePause(SOURCE(mus));
}

void
music_resume(struct music *mus)
{
	assert(music_ok(mus));

	alSourcePlay(SOURCE(mus));
}

void
music_stop(struct music *mus)
{
	assert(music_ok(mus));

	alSourceStop(SOURCE(mus));
}

void
music_finish(struct music *mus)
{
	assert(mus);

	if (mus->handle) {
		music_stop(mus);
		audiostream_finish(mus->handle);
	}

	memset(mus, 0, sizeof (*mus));
}

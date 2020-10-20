/*
 * sound.c -- sound support
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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

#include <SDL_mixer.h>

#include "error.h"
#include "sound.h"

bool
sound_open(struct sound *snd, const char *path)
{
	assert(snd);
	assert(path);

	if (!(snd->handle = Mix_LoadMUS(path)))
		return errorf("%s", SDL_GetError());

	return true;
}

bool
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz)
{
	assert(snd);
	assert(buffer);

	SDL_RWops *ops;

	if (!(ops = SDL_RWFromConstMem(buffer, buffersz)) ||
	    !(snd->handle = Mix_LoadMUS_RW(ops, true)))
		return errorf("%s", SDL_GetError());

	return true;
}

bool
sound_play(struct sound *snd)
{
	assert(snd);

	int n = 1;

	if (snd->flags & SOUND_LOOP)
		n = -1;
	if (Mix_PlayMusic(snd->handle, n) < 0)
		return errorf("%s", SDL_GetError());

	return true;
}

void
sound_pause(struct sound *snd)
{
	/* Not needed yet. */
	(void)snd;

	Mix_PauseMusic();
}

void
sound_resume(struct sound *snd)
{
	/* Not needed yet. */
	(void)snd;

	Mix_ResumeMusic();
}

void
sound_stop(struct sound *snd)
{
	/* Not needed yet. */
	(void)snd;

	Mix_HaltMusic();
}

void
sound_finish(struct sound *snd)
{
	assert(snd);

	if (snd->handle) {
		Mix_HaltMusic();
		Mix_FreeMusic(snd->handle);
	}

	memset(snd, 0, sizeof (*snd));
}

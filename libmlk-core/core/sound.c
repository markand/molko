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

	if (!(snd->handle = Mix_LoadWAV(path)))
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
	    !(snd->handle = Mix_LoadWAV_RW(ops, true)))
		return errorf("%s", SDL_GetError());

	return true;
}

bool
sound_ok(const struct sound *snd)
{
	return snd && snd->handle;
}

bool
sound_play(struct sound *snd, int channel, unsigned int fadein)
{
	assert(sound_ok(snd));

	int ret;

	if (fadein > 0)
		ret = Mix_FadeInChannel(channel, snd->handle, 0, fadein);
	else
		ret = Mix_PlayChannel(channel, snd->handle, 0);

	if (ret < 0)
		return errorf("%s", SDL_GetError());

	snd->channel = channel;

	return true;
}

void
sound_pause(struct sound *snd)
{
	Mix_Pause(snd ? snd->channel : -1);
}

void
sound_resume(struct sound *snd)
{
	Mix_Resume(snd ? snd->channel : -1);
}

void
sound_stop(struct sound *snd, unsigned int fadeout)
{
	if (fadeout > 0)
		Mix_FadeOutChannel(snd ? snd->channel : -1, fadeout);
	else
		Mix_HaltChannel(snd ? snd->channel : -1);
}

void
sound_finish(struct sound *snd)
{
	assert(snd);

	if (snd->handle) {
		Mix_HaltChannel(snd->channel);
		Mix_FreeChunk(snd->handle);
	}

	memset(snd, 0, sizeof (*snd));
}

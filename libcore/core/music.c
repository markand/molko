/*
 * music.h -- music support
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
#include <string.h>

#include <SDL_mixer.h>

#include "error.h"
#include "music.h"

bool
music_open(struct music *mus, const char *path)
{
	assert(mus);
	assert(path);

	if (!(mus->handle = Mix_LoadMUS(path)))
		return errorf("%s", SDL_GetError());

	return true;
}

bool
music_openmem(struct music *mus, const void *buffer, size_t buffersz)
{
	assert(mus);
	assert(buffer);

	SDL_RWops *ops;

	if (!(ops = SDL_RWFromConstMem(buffer, buffersz)) ||
	    !(mus->handle = Mix_LoadMUS_RW(ops, true)))
		return errorf("%s", SDL_GetError());

	return true;
}

bool
music_ok(const struct music *mus)
{
	return mus && mus->handle;
}

bool
music_play(struct music *mus, enum music_flags flags, unsigned int fadein)
{
	assert(mus);

	int loops = flags & MUSIC_LOOP ? -1 : 1;
	int ret;

	if (fadein > 0)
		ret = Mix_FadeInMusic(mus->handle, loops, fadein);
	else
		ret = Mix_PlayMusic(mus->handle, loops);

	if (ret < 0)
		return errorf("%s", SDL_GetError());

	return true;
}

bool
music_playing(void)
{
	return Mix_PlayingMusic();
}

void
music_pause(void)
{
	Mix_PauseMusic();
}

void
music_resume(void)
{
	Mix_ResumeMusic();
}

void
music_stop(unsigned int fadeout)
{
	if (fadeout > 0)
		Mix_FadeOutMusic(fadeout);
	else
		Mix_HaltMusic();
}

void
music_finish(struct music *mus)
{
	assert(mus);

	if (mus->handle) {
		Mix_HaltMusic();
		Mix_FreeMusic(mus->handle);
	}

	memset(mus, 0, sizeof (*mus));
}

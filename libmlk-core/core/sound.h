/*
 * sound.h -- sound support
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

#ifndef MOLKO_CORE_SOUND_H
#define MOLKO_CORE_SOUND_H

#include <stdbool.h>
#include <stddef.h>


#define SOUND_CHANNELS_MAX (256)

struct sound {
	void *handle;
	int channel;
};

bool
sound_open(struct sound *snd, const char *path);

bool
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz);

bool
sound_ok(const struct sound *snd);

bool
sound_play(struct sound *snd, int channel, unsigned int fadein);

void
sound_pause(struct sound *snd);

void
sound_resume(struct sound *snd);

void
sound_stop(struct sound *snd, unsigned int fadeout);

void
sound_finish(struct sound *snd);

#endif /* !MOLKO_CORE_SOUND_H */

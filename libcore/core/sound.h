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

/**
 * \file sound.h
 * \brief Sound support.
 */

#include <stdbool.h>
#include <stddef.h>

#include "plat.h"

/**
 * \brief Number of channels allocated.
 */
#define SOUND_MAX_CHANNELS	(256)

/**
 * \brief Sound chunk.
 */
struct sound {
	void *handle;                   /*!< (*) Native handle. */
	int channel;                    /*!< (*) Current channel. */
};

/**
 * Open a sound audio file.
 *
 * \pre snd != NULL
 * \pre path != NULL
 * \param snd the sound object to initialize
 * \param path the path to the audio file
 * \return False on errors.
 */
bool
sound_open(struct sound *snd, const char *path) PLAT_NODISCARD;

/**
 * Open a sound audio from a buffer.
 *
 * \pre snd != NULL
 * \pre buffer != NULL
 * \param snd the sound object to initialize
 * \param buffer the buffer
 * \param buffersz the buffer size
 * \return False on errors.
 * \warning The buffer must exists until the sound object is closed.
 */
bool
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz) PLAT_NODISCARD;

/**
 * Check if this sound handle is properly loaded.
 *
 * \param snd the sound to check (may be NULL)
 */
bool
sound_ok(const struct sound *snd);

/**
 * Start playing the sound.
 *
 * This function will resume the playback since the beginning.
 *
 * \pre sound_ok(snd)
 * \param snd the sound object
 * \param channel the channel to use (-1 for a default)
 * \param fadein a fade in delay in milliseconds (0 to disable)
 * \return False on errors.
 */
bool
sound_play(struct sound *snd, int channel, unsigned int fadein);

/**
 * Pause the given sound or all sounds currently playing.
 *
 * \param snd the sound object (or NULL to pause all)
 */
void
sound_pause(struct sound *snd);

/**
 * Resume the current sound or all sounds currently paused.
 *
 * \param snd the sound object (or NULL to resume all)
 */
void
sound_resume(struct sound *snd);

/**
 * Stop the sound music or all sounds currently playing.
 *
 * \pre sound_ok(snd)
 * \param snd the sound object
 * \param fadeout a fade out delay in milliseconds (0 to disable)
 */
void
sound_stop(struct sound *snd, unsigned int fadeout);

/**
 * Close the associated resources. This will also stop the playback of the
 * given sound.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
sound_finish(struct sound *snd);

#endif /* !MOLKO_CORE_SOUND_H */

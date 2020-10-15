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

#ifndef MOLKO_SOUND_H
#define MOLKO_SOUND_H

/**
 * \file sound.h
 * \brief Sound support.
 */

#include <stdbool.h>
#include <stddef.h>

/**
 * \brief Sound flags.
 */
enum sound_flags {
	SOUND_NONE,                     /*!< No flags. */
	SOUND_LOOP      = (1 << 0)      /*!< Loop the music. */
};

/**
 * \brief Sound chunk.
 */
struct sound {
	enum sound_flags flags;         /*!< (+) Flags. */
	void *handle;                   /*!< (*) Native handle. */
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
sound_open(struct sound *snd, const char *path);

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
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz);

/**
 * Start playing the sound.
 *
 * This function will resume the playback since the beginning.
 *
 * \pre snd != NULL
 * \param snd the sound object
 * \return False on errors.
 */
bool
sound_play(struct sound *snd);

/**
 * Pause the sound music.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
sound_pause(struct sound *snd);

/**
 * Resume the sound music.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
sound_resume(struct sound *snd);

/**
 * Stop the sound music.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
sound_stop(struct sound *snd);

/**
 * Close the associated resources.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
sound_finish(struct sound *snd);

#endif /* !MOLKO_SOUND_H */

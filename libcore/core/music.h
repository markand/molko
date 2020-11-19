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

#ifndef MOLKO_CORE_MUSIC_H
#define MOLKO_CORE_MUSIC_H

/**
 * \file music.h
 * \brief Music support.
 *
 * This module provide support for playing music. In contrast to sounds only one
 * music can be played at a time.
 */

#include <stdbool.h>
#include <stddef.h>

/**
 * \brief Music flags.
 */
enum music_flags {
	MUSIC_NONE,                     /*!< No flags. */
	MUSIC_LOOP      = (1 << 0)      /*!< Loop the music. */
};

/**
 * \brief Music object handle.
 */
struct music {
	void *handle;                   /*!< (*) Implementation handle. */
};

/**
 * Open a music file.
 *
 * \pre mus != NULL
 * \pre path != NULL
 * \param mus the music object to initialize
 * \param path the path to the music file
 * \return False on errors.
 */
bool
music_open(struct music *mus, const char *path);

/**
 * Open a music from a buffer.
 *
 * \pre mus != NULL
 * \pre buffer != NULL
 * \param mus the music object to initialize
 * \param buffer the buffer
 * \param buffersz the buffer size
 * \return False on errors.
 * \warning The buffer must exists until the sound object is closed.
 */
bool
music_openmem(struct music *mus, const void *buffer, size_t buffersz);

/**
 * Check if this music handle is properly loaded.
 *
 * \param mus the music to check (may be NULL)
 */
bool
music_ok(const struct music *mus);

/**
 * Start playing the given music
 *
 * This function will resume the playback since the beginning and will stop the
 * current music. If the music playing is currently fading out this function
 * will block until it has finished.
 *
 * \pre mus != NULL
 * \param mus the music to start
 * \param flags optional flags
 * \param fadein a fade in delay in milliseconds (0 to disable)
 * \return False on errors.
 */
bool
music_play(struct music *mus, enum music_flags flags, unsigned int fadein);

/**
 * Tells if music is playing.
 *
 * You can call this function with any music even one that is not currently
 * playing.
 */
bool
music_playing(void);

/**
 * Pause the music playback.
 */
void
music_pause(void);

/**
 * Resume the music playback.
 */
void
music_resume(void);

/**
 * Stop the sound music.
 *
 * \param fadeout a fade out delay in milliseconds (0 to disable)
 */
void
music_stop(unsigned int fadeout);

/**
 * Close the associated resources.
 *
 * \pre snd != NULL
 * \param snd the sound object
 */
void
music_finish(struct music *mus);

#endif /* !MOLKO_CORE_MUSIC_H */

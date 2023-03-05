/*
 * music.h -- music support
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

#ifndef MLK_CORE_MUSIC_H
#define MLK_CORE_MUSIC_H

/**
 * \file mlk/core/music.h
 * \brief Music support
 *
 * This module provides support for loading music files and play them with
 * various features such as looping.
 */

#include <stddef.h>

/**
 * \enum mlk_music_flags
 * \brief Music flags
 *
 * This enumeration is implemented as a bitmask.
 */
enum mlk_music_flags {
	/**
	 * No flags.
	 */
	MLK_MUSIC_NONE  = 0,

	/**
	 * Loop the music.
	 */
	MLK_MUSIC_LOOP  = (1 << 0)
};

/**
 * \struct mlk_music
 * \brief Music structure
 *
 * This structure is non-opaque but has no public fields.
 */
struct mlk_music {
	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open a music from the file system path.
 *
 * \pre music != NULL
 * \pre path != NULL
 * \param music the music to initialize
 * \param path the path to the music file (e.g. .ogg, .wav, .mp3, etc)
 * \return 0 on success or an error code on failure
 */
int
mlk_music_open(struct mlk_music *music, const char *path);

/**
 * Open a music from a const binary data.
 *
 * The binary data must be kept alive until the music is no longer used.
 *
 * \pre music != NULL
 * \pre path != NULL
 * \param music the music to initialize
 * \param data the font content
 * \param datasz the font content length
 * \return 0 on success or an error code on failure
 */
int
mlk_music_openmem(struct mlk_music *music, const void *data, size_t datasz);

/**
 * Tells if the music structure is usable.
 *
 * \param music the music to check
 * \return non-zero if the music structure is usable
 */
int
mlk_music_ok(const struct mlk_music *music);

/**
 * Start playing the music.
 *
 * \pre mlk_music_ok(music)
 * \param music the music to play
 * \param flags optional flags to pass
 * \return 0 on success or an error code on failure
 */
int
mlk_music_play(struct mlk_music *music, enum mlk_music_flags flags);

/**
 * Pause the music playback.
 *
 * \pre mlk_music_ok(music)
 * \param music the music to pause
 * \sa ::mlk_music_resume
 */
void
mlk_music_pause(struct mlk_music *music);

/**
 * Resume the music where it was stopped.
 *
 * \pre mlk_music_ok(music)
 * \param music the music to resume
 * \sa ::mlk_music_pause
 */
void
mlk_music_resume(struct mlk_music *music);

/**
 * Stop and rewind the music.
 *
 * Calling ::mlk_music_resume on it will restart from the beginning.
 *
 * \pre mlk_music_ok(music)
 * \param music the music to stop
 * \sa ::mlk_music_resume
 * \sa ::mlk_music_play
 */
void
mlk_music_stop(struct mlk_music *music);

/**
 * Destroy this music.
 *
 * If the music is being played, it is stopped immediately.
 *
 * \pre mlk_music_ok(music)
 * \param music the music to destroy
 */
void
mlk_music_finish(struct mlk_music *music);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_MUSIC_H */

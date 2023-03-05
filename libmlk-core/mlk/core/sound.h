/*
 * sound.h -- sound support
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

#ifndef MLK_CORE_SOUND_H
#define MLK_CORE_SOUND_H

/**
 * \file mlk/core/sound.h
 * \brief Sound support
 *
 * This module provides API to play sounds.
 *
 * In contrast to music.h module, multiple sounds can be played at a time and
 * can't be looped.
 */

#include <stddef.h>

/**
 * \struct mlk_music
 * \brief Music structure
 *
 * This structure is non-opaque but has no public fields.
 */
struct mlk_sound {
	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open a sound file from the file system path.
 *
 * \pre music != NULL
 * \pre path != NULL
 * \param sound the sound to initialize
 * \param path the path to the music file (e.g. .ogg, .wav, .mp3, etc)
 * \return 0 on success or an error code on failure
 */
int
mlk_sound_open(struct mlk_sound *sound, const char *path);

/**
 * Open a sound from a const binary data.
 *
 * The binary data must be kept alive until the sound is no longer used.
 *
 * \pre music != NULL
 * \pre path != NULL
 * \param sound the sound to initialize
 * \param data the font content
 * \param datasz the font content length
 * \return 0 on success or an error code on failure
 */
int
mlk_sound_openmem(struct mlk_sound *sound, const void *data, size_t datasz);

/**
 * Tells if the sound structure is usable.
 *
 * \param sound the sound to check
 * \return non-zero if the sound structure is usable
 */
int
mlk_sound_ok(const struct mlk_sound *sound);

/**
 * Start playing the sound.
 *
 * \pre mlk_sound_ok(sound)
 * \param sound the sound to play
 * \return 0 on success or an error code on failure
 */
int
mlk_sound_play(struct mlk_sound *sound);

/**
 * Pause the sound playback.
 *
 * \pre mlk_sound_ok(sound)
 * \param sound the sound to pause
 * \sa ::mlk_sound_resume
 */
void
mlk_sound_pause(struct mlk_sound *sound);

/**
 * Resume the sound where it was stopped.
 *
 * \pre mlk_sound_ok(sound)
 * \param sound the sound to resume
 * \sa ::mlk_sound_pause
 */
void
mlk_sound_resume(struct mlk_sound *sound);

/**
 * Stop and rewind the sound.
 *
 * Calling ::mlk_sound_resume on it will restart from the beginning.
 *
 * \pre mlk_sound_ok(sound)
 * \param sound the sound to stop
 * \sa ::mlk_sound_resume
 * \sa ::mlk_sound_play
 */
void
mlk_sound_stop(struct mlk_sound *sound);

/**
 * Destroy this sound.
 *
 * If the sound is being played, it is stopped immediately.
 *
 * \pre mlk_sound_ok(sound)
 * \param sound the sound to destroy
 */
void
mlk_sound_finish(struct mlk_sound *sound);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_SOUND_H */

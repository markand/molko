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

#include <stddef.h>

#include "core.h"

#define MLK_SOUND_CHANNELS_MAX (256)

struct mlk_sound {
	void *handle;
	int channel;
};

MLK_CORE_BEGIN_DECLS

int
mlk_sound_open(struct mlk_sound *, const char *);

int
mlk_sound_openmem(struct mlk_sound *, const void *, size_t);

int
mlk_sound_ok(const struct mlk_sound *);

int
mlk_sound_play(struct mlk_sound *);

void
mlk_sound_pause(struct mlk_sound *);

void
mlk_sound_resume(struct mlk_sound *);

void
mlk_sound_stop(struct mlk_sound *);

void
mlk_sound_finish(struct mlk_sound *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_SOUND_H */

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

#include <stdbool.h>
#include <stddef.h>

#include "core.h"

enum music_flags {
	MUSIC_NONE      = 0,
	MUSIC_LOOP      = (1 << 0)
};

struct music {
	void *handle;
};

CORE_BEGIN_DECLS

bool
music_open(struct music *mus, const char *path);

bool
music_openmem(struct music *mus, const void *buffer, size_t buffersz);

bool
music_ok(const struct music *mus);

bool
music_play(struct music *mus, enum music_flags flags, unsigned int fadein);

bool
music_playing(void);

void
music_pause(void);

void
music_resume(void);

void
music_stop(unsigned int fadeout);

void
music_finish(struct music *mus);

CORE_END_DECLS

#endif /* !MOLKO_CORE_MUSIC_H */

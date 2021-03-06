/*
 * music.h -- music support
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include <stddef.h>

#include "core.h"

struct vfs_file;

enum music_flags {
	MUSIC_NONE      = 0,
	MUSIC_LOOP      = (1 << 0)
};

struct music {
	void *handle;
};

CORE_BEGIN_DECLS

int
music_open(struct music *, const char *);

int
music_openmem(struct music *, const void *, size_t);

int
music_openvfs(struct music *, struct vfs_file *);

int
music_ok(const struct music *);

int
music_play(struct music *, enum music_flags);

void
music_pause(struct music *);

void
music_resume(struct music *);

void
music_stop(struct music *);

void
music_finish(struct music *);

CORE_END_DECLS

#endif /* !MLK_CORE_MUSIC_H */

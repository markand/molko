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

#include <stddef.h>

#include "core.h"

enum mlk_music_flags {
	MLK_MUSIC_NONE  = 0,
	MLK_MUSIC_LOOP  = (1 << 0)
};

struct mlk_music {
	void *handle;
};

MLK_CORE_BEGIN_DECLS

int
mlk_music_open(struct mlk_music *, const char *);

int
mlk_music_openmem(struct mlk_music *, const void *, size_t);

int
mlk_music_ok(const struct mlk_music *);

int
mlk_music_play(struct mlk_music *, enum mlk_music_flags);

void
mlk_music_pause(struct mlk_music *);

void
mlk_music_resume(struct mlk_music *);

void
mlk_music_stop(struct mlk_music *);

void
mlk_music_finish(struct mlk_music *);

MLK_CORE_END_DECLS

#endif /* !MLK_CORE_MUSIC_H */

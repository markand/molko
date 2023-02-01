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

#define SOUND_CHANNELS_MAX (256)

struct vfs_file;

struct sound {
	void *handle;
	int channel;
};

CORE_BEGIN_DECLS

int
sound_open(struct sound *, const char *);

int
sound_openmem(struct sound *, const void *, size_t);

int
sound_openvfs(struct sound *, struct vfs_file *);

int
sound_ok(const struct sound *);

int
sound_play(struct sound *);

void
sound_pause(struct sound *);

void
sound_resume(struct sound *);

void
sound_stop(struct sound *);

void
sound_finish(struct sound *);

CORE_END_DECLS

#endif /* !MLK_CORE_SOUND_H */

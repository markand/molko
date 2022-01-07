/*
 * sys_p.h -- libcore private definitions
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

#ifndef MLK_CORE_SYS_P_H
#define MLK_CORE_SYS_P_H

#include <stddef.h>

#include <al.h>
#include <alc.h>
#include <alext.h>

extern ALCdevice *audio_dev;
extern ALCcontext *audio_ctx;

struct audiostream {
	short *samples;
	ALsizei samplesz;
	ALsizei samplerate;
	ALuint buffer;
	ALuint source;
	ALenum format;
};

struct audiostream *
audiostream_open(const char *);

struct audiostream *
audiostream_openmem(const void *, size_t);

void
audiostream_finish(struct audiostream *);

#endif /* !MLK_CORE_SYS_P_H */

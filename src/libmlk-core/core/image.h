/*
 * image.h -- basic image management
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

#ifndef MLK_CORE_IMAGE_H
#define MLK_CORE_IMAGE_H

#include <stddef.h>

#include "core.h"

struct texture;
struct vfs_file;

CORE_BEGIN_DECLS

int
image_open(struct texture *, const char *);

int
image_openmem(struct texture *, const void *, size_t);

int
image_openvfs(struct texture *, struct vfs_file *);


CORE_END_DECLS

#endif /* !MLK_CORE_IMAGE_H */

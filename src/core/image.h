/*
 * image.h -- basic image management
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

#ifndef MOLKO_IMAGE_H
#define MOLKO_IMAGE_H

/**
 * \file image.h
 * \brief Basic image management.
 * \ingroup drawing
 */

#include <stddef.h>

struct texture;

/**
 * Open a file from a path.
 *
 * \pre tex != NULL
 * \pre path != NULL
 * \param tex the texture to initialize
 * \param path the path to the file
 * \return False on errors.
 */
bool
image_open(struct texture *tex, const char *path);

/**
 * Open a file from a memory buffer.
 *
 * The buffer must be valid until the image is no longer used.
 *
 * \pre tex != NULL
 * \pre buffer != NULL
 * \param tex the texture to initialize
 * \param buffer the memory buffer
 * \param size the memory size
 * \return False on errors.
 */
bool
image_openmem(struct texture *tex, const void *buffer, size_t size);

#endif /* !MOLKO_IMAGE_H */

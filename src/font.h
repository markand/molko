/*
 * font.h -- basic font management
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

#ifndef MOLKO_FONT_H
#define MOLKO_FONT_H

/**
 * \file font.h
 * \brief Basic font management.
 */

#include <stddef.h>

/**
 * \brief Font object.
 *
 * This object is not publicly defined because it contains
 * implementation-defined data.
 */
struct font;
struct texture;

/**
 * Open font from path file.
 *
 * \pre path != NULL
 * \param path the path to the font
 * \param size the desired size
 * \return the font or NULL on error
 */
struct font *
font_openf(const char *path, unsigned int size);

/**
 * Open font from memory buffer.
 *
 * \pre buffer != NULL
 * \param buffer the memory buffer
 * \param buflen the memory buffer length
 * \param size the desired size
 * \warning The buffer must remain valid until font is closed
 * \return the font or NULL on error
 */
struct font *
font_openb(const void *buffer, size_t buflen, unsigned int size);

/**
 * Render a text.
 *
 * \pre font != NULL
 * \pre text != NULL
 * \param font the font handle
 * \param text the text in UTF-8
 * \param color the color
 */
struct texture *
font_render(struct font *font, const char *text, unsigned long color);

/**
 * Close the font.
 *
 * \pre font != NULL
 * \param font the font handle
 */
void
font_close(struct font *font);

#endif /* !MOLKO_FONT_H */

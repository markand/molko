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
 * \ingroup drawing
 */

#include <stdbool.h>
#include <stddef.h>

struct texture;

/**
 * \brief Font style for rendering.
 */
enum font_style {
	FONT_STYLE_ANTIALIASED,         /*!< Pretty antialiasing looking. */
	FONT_STYLE_NONE                 /*!< No antialiasing. */
};

/**
 * \brief Font object.
 */
struct font {
	enum font_style style;          /*!< (RW) Style for rendering. */
	unsigned long color;            /*!< (RW) Color for rendering. */
	void *handle;                   /*!< (RO) Native handle. */
};

/**
 * Open font from path file.
 *
 * \pre font != NULL
 * \pre path != NULL
 * \param font the font to initialize
 * \param path the path to the font
 * \param size the desired size
 * \return False on errors.
 */
bool
font_open(struct font *font, const char *path, unsigned int size);

/**
 * Open font from memory buffer.
 *
 * \pre font != NULL
 * \pre buffer != NULL
 * \param font the font to initialize
 * \param buffer the memory buffer
 * \param buflen the memory buffer length
 * \param size the desired size
 * \warning The buffer must remain valid until font is closed
 * \return False on errors.
 */
bool
font_openmem(struct font *font, const void *buffer, size_t buflen, unsigned int size);

/**
 * Tells if the font was properly opened.
 *
 * \pre font != NULL
 * \param font the font to check
 * \return True if the native handle was opened.
 */
bool
font_ok(const struct font *font);

/**
 * Render some text into the texture.
 *
 * This function use the current color/style and other properties in the font
 * to render the texture.
 *
 * \pre font != NULL
 * \pre tex != NULL
 * \param font the font to use
 * \param tex the texture to generate
 * \param text the UTF-8 text
 * \return False on errors.
 */
bool
font_render(struct font *font, struct texture *tex, const char *text);

/**
 * Get the maximum height for all glyphs.
 *
 * \pre font != NULL
 * \param font the font handle
 * \return the height in pixels
 */
unsigned int
font_height(const struct font *font);

/**
 * Close the font.
 *
 * \pre font != NULL
 * \param font the font handle
 */
void
font_finish(struct font *font);

#endif /* !MOLKO_FONT_H */

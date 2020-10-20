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
	enum font_style style;          /*!< (+) Style for rendering. */
	unsigned long color;            /*!< (+) Color for rendering. */
	void *handle;                   /*!< (*) Native handle. */
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
 * Convenient shortcut to shallow copy src into dst.
 *
 * Use this function when you want your own local font to update its attributes
 * without changing the calling site.
 *
 * This is a shortcut to `memcpy(dst, src, sizeof (*src))`.
 *
 * \pre dst != NULL
 * \param dst the destination font
 * \param src the source font (may be NULL)
 * \note Internal handle points to the same pointer, do not use \ref font_finish
 *       on the destination.
 */
void
font_shallow(struct font *dst, const struct font *src);

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
 * \pre font_ok(font)
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
 * \pre font_ok(font)
 * \param font the font handle
 * \return the height in pixels
 */
unsigned int
font_height(const struct font *font);

/**
 * Compute the text size required with this font.
 *
 * \pre font_ok(font)
 * \pre text != NULL
 * \param font the font object
 * \param text the UTF-8 text
 * \param w pointer to width (may be NULL)
 * \param h pointer to height (may be NULL)
 * \return False in case of error and pointers to w and h are left unmodified.
 */
bool
font_query(const struct font *font, const char *text, unsigned int *w, unsigned int *h);

/**
 * Close the font.
 *
 * \pre font != NULL
 * \param font the font handle
 */
void
font_finish(struct font *font);

#endif /* !MOLKO_FONT_H */

/*
 * font.h -- basic font management
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

#ifndef MLK_CORE_FONT_H
#define MLK_CORE_FONT_H

/**
 * \file mlk/core/font.h
 * \brief Basic font management
 *
 * Open and use fonts for rendering UTF-8 text.
 */

#include <stddef.h>

struct mlk_texture;

/**
 * \enum mlk_font_style
 * \brief Font style
 */
enum mlk_font_style {
	/**
	 * Rendered text is antialiased, default.
	 */
	MLK_FONT_STYLE_ANTIALIASED,

	/**
	 * Simple rendering, no antialiasing.
	 */
	MLK_FONT_STYLE_NONE,

	/**
	 * Last enumeration constant.
	 */
	MLK_FONT_STYLE_LAST
};

/**
 * \struct mlk_font
 * \brief Font structure
 *
 * You can fill font attributes before calling ::mlk_font_open or
 * ::mlk_font_openmem.
 */
struct mlk_font {
	/**
	 * (read-write)
	 *
	 * Style used for rendering, can be changed at runtime.
	 */
	enum mlk_font_style style;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open a font from the file system path.
 *
 * \pre font != NULL
 * \pre path != NULL
 * \param font the font to open
 * \param path path to the font file (e.g. .ttf, .otf, etc)
 * \param size desired font height in pixels
 * \return 0 on success or an error code on failure
 */
int
mlk_font_open(struct mlk_font *font, const char *path, unsigned int size);

/**
 * Open a font from a const binary data.
 *
 * The binary data must be kept alive until the font is no longer used.
 *
 * \pre font != NULL
 * \param font the font to open
 * \param data the font content
 * \param datasz the font content length
 * \param size desired font height in pixels
 * \return 0 on success or an error code on failure
 */
int
mlk_font_openmem(struct mlk_font *font,
                 const void *data,
                 size_t datasz,
                 unsigned int size);

/**
 * Render some text using the font and generate a texture.
 *
 * The texture destination must be deallocated once no longer used using
 * ::mlk_texture_finish.
 *
 * \pre mlk_font_ok(font)
 * \pre texture != NULL
 * \pre text != NULL && strlen(text) > 0
 * \param font the font to use
 * \param texture the texture to initialize
 * \param text the non NULL and non empty UTF-8 text
 * \param color foreground color
 * \return 0 on success or an error code on failure
 */
int
mlk_font_render(struct mlk_font *font,
                struct mlk_texture *texture,
                const char *text,
                unsigned long color);

/**
 * Return the font height in pixels
 *
 * \pre mlk_font_ok(font)
 * \param font the font to use
 * \return the font height
 */
unsigned int
mlk_font_height(const struct mlk_font *font);

/**
 * Query a dimension required to draw a text.
 *
 * Both w and h pointers can be NULL if one of the dimension isn't required.
 *
 * If the function fails, *w and *h are set to 0.
 *
 * \pre mlk_font_ok(font)
 * \param font the font to use
 * \param text the non NULL and non empty UTF-8 text
 * \param w pointer receiving width (or NULL)
 * \param h pointer receiving height (or NULL)
 * \return 0 on success or an error code on failure
 */
int
mlk_font_query(const struct mlk_font *font,
               const char *text,
               unsigned int *w,
               unsigned int *h);

/**
 * Destroy this font.
 *
 * \pre mlk_font_ok(font)
 * \param font the font to destroy
 */
void
mlk_font_finish(struct mlk_font *);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_FONT_H */

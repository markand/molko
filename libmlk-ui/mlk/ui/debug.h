/*
 * debug.h -- debugging interfaces
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

#ifndef MLK_UI_DEBUG_H
#define MLK_UI_DEBUG_H

/**
 * \file mlk/ui/debug.h
 * \brief Debugging interfaces.
 *
 * This module provides simple functions to draw messages on the top of the
 * screen aimed at showing debugging informations. It is simple by design as it
 * should be just called in your draw function to render text with some style.
 *
 * In your draw loop, you have to re-use a ::mlk_debug_report object to count up
 * how many lines the module has to draw.
 *
 * Example of use in draw function:
 *
 * ```c
 * struct mlk_debug_report report = {};
 *
 * mlk_debugf(&report, "Game running.");
 * mlk_debugf(&report, "mouse: %d, %d", my_mouse_x, my_mouse_y);
 * ```
 */

#include <stdarg.h>

#define MLK_DEBUG_LINE_MAX 256

struct mlk_font;

/**
 * \struct mlk_debug_style
 * \brief Controls the style to use.
 */
struct mlk_debug_style {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Font to use, defaults to ::MLK_UI_FONT_INTERFACE.
	 */
	struct mlk_font *font;

	/**
	 * (read-write)
	 *
	 * Text color
	 */
	unsigned long color;

	/**
	 * (read-write)
	 *
	 * Padding between elements and top left (x, y) coordinates.
	 */
	unsigned int padding;

	/**
	 * (read-write, optional)
	 *
	 * Custom drawing function.
	 *
	 * \param line the text line to show
	 * \param n the current line number
	 */
	void (*draw)(const char *line, unsigned int n);
};

/**
 * \struct mlk_debug_options
 * \brief Debug interface options.
 */
struct mlk_debug_options {
	/**
	 * (read-write)
	 *
	 * Enable the module at runtime, defauls to 1 if built as non-release
	 * which means with NDEBUG **not** set.
	 */
	int enable;
};

/**
 * \struct mlk_debug_report
 * \brief Opaque data type to carry for each call.
 */
struct mlk_debug_report {
	/** \cond MLK_PRIVATE_DECLS */
	unsigned int count;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \brief Global debugging options.
 */
extern struct mlk_debug_options mlk_debug_options;

/**
 * \brief Global style options.
 */
extern struct mlk_debug_style mlk_debug_style;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Render a debugging message using a printf(3) format string, no-op if
 * ::mlk_debug_options::enable is zero.
 *
 * \pre report != NULL
 * \pre fmt != NULL
 * \param report opaque report object to pass across calls
 * \param fmt the printf(3) format string
 */
void
mlk_debugf(struct mlk_debug_report *report, const char *fmt, ...);

/**
 * Similar to ::mlk_debugf but using a `va_list` instead.
 */
void
mlk_debugva(struct mlk_debug_report *report, const char *fmt, va_list ap);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_UI_DEBUG_H */

/*
 * debug.h -- debugging interfaces
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

#ifndef MOLKO_DEBUG_H
#define MOLKO_DEBUG_H

/**
 * \file debug.h
 * \brief Debugging interfaces.
 *
 * This module provides functions to draw debugging information within the game
 * window. It is mostly used for information only when state of the game is
 * better inspected via direct window information rather than writing in the
 * console.
 *
 * Predefined core states may print debugging information if
 * debug_options.enabled variable is set to true. However, you need to specify
 * the font to use in order to work.
 *
 * Each call to \ref debug_printf or \ref debug_vprintf automatically adjust
 * next coordinate for rendering the text. As such, user may simply print
 * several lines of text without having to deal with that manually.
 *
 * Example, activate global debugging.
 *
 * \code
 * struct font *f = font_openf("foo.ttf", 10);
 *
 * if (!f)
 * 	error_fatal();
 *
 * debug_options.default_font = f;
 * debug_options.enabled = true;
 * \endcode
 *
 * Example, print debugging information manually.
 *
 * \code
 * struct debug_report report = {
 * 	.color = 0x00ff00ff,
 * 	.font = myfont          // Optional if debug_options.default_font is set.
 * };
 *
 * debug_printf("current position: %d, %d\n", x, y);
 * \endcode
 */

#include <stdbool.h>
#include <stdarg.h>

#include "font.h"

/**
 * Maximum content length per report.
 */
#define DEBUG_LINE_MAX 1024

/**
 * \brief Debugging options.
 *
 * Fill this structure with appropriate values to change debugging behavior
 * in core API.
 */
struct debug_options {
	struct font *default_font;      /*!< (RW) Default font for reports. */
	enum font_style default_style;  /*!< (RW) Default font style. */
	unsigned long default_color;    /*!< (RW) Default font color. */
	bool enable;                    /*!< (RW) Enable core API debugging. */
};

/**
 * \brief Debug context.
 *
 * Use this structure each time you need to print one or more messages.
 */
struct debug_report {
	struct font *font;              /*!< (RW) Font to use. */
	enum font_style style;          /*!< (RW) Font style to use. */
	unsigned long color;            /*!< (RW) Font foreground color to use. */
	unsigned int count;             /*!< (PV) Number of messages already printed. */
};

/**
 * Global debugging options.
 */
extern struct debug_options debug_options;

/**
 * Convenient macro to initialize \ref debug_report with default values.
 */
#define DEBUG_INIT_DEFAULTS {                   \
        .font = debug_options.default_font,     \
        .color = debug_options.default_color,   \
        .style = debug_options.default_style    \
}

/**
 * Print debugging information into the screen.
 *
 * \pre report != NULL
 * \pre fmt != NULL
 * \param report the reporting context
 * \param fmt the printf(3) format string
 * \note The message length must not exceed DEBUG_LINE_MAX, otherwise its
 *       result is truncated.
 */
void
debug_printf(struct debug_report *report, const char *fmt, ...);

/**
 * Similar to \ref debug_printf but with a va_list object.
 *
 * \see \ref debug_printf
 */
void
debug_vprintf(struct debug_report *report, const char *fmt, va_list ap);

#endif /* !MOLKO_DEBUG_H */

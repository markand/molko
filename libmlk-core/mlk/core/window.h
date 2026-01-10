/*
 * window.h -- basic window management
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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

#ifndef MLK_CORE_WINDOW_H
#define MLK_CORE_WINDOW_H

/**
 * \file window.h
 * \brief Basic window management
 *
 * Main window module.
 *
 * In Molko's Engine, only one window can be opened at a time and is provided
 * read-only with the ::mlk_window global variable.
 */

/**
 * \enum mlk_window_theme
 * \brief Operating system theme.
 *
 * This enumeration is used for both user preference and current effective theme
 * which are two distinct things. If user selects `auto` then the effective
 * theme will be set either to light or dark depending on the system platform.
 */
enum mlk_window_theme {
	/**
	 * Prefer the operating system user preference, if theme can't be
	 * detected it will be set to light instead.
	 */
	MLK_WINDOW_THEME_AUTO,

	/**
	 * Light theme.
	 */
	MLK_WINDOW_THEME_LIGHT,

	/**
	 * Dark theme.
	 */
	MLK_WINDOW_THEME_DARK,
	
	/**
	 * Unused sentinel value.
	 */
	MLK_WINDOW_THEME_LAST
};

/**
 * \struct mlk_window
 * \brief Window structure
 */
struct mlk_window {
	/**
	 * (read-only)
	 *
	 * Window width.
	 */
	unsigned int w;

	/**
	 * (read-only)
	 *
	 * Window height.
	 */
	unsigned int h;

	/**
	 * (read-only)
	 *
	 * Display preferred framerate. May be set to 0 if not supported.
	 */
	unsigned int framerate;

	/**
	 * (read-only)
	 *
	 * User preferred theme.
	 *
	 * \warning Do not set this variable manually, it won't have any effect
	 *          use ::mlk_window_set_theme instead.
	 */
	enum mlk_window_theme theme_user;

	/**
	 * (read-only)
	 *
	 * Current effective theme depending on the user preferred setting, this
	 * value holds either ::MLK_WINDOW_THEME_LIGHT or
	 * ::MLK_WINDOW_THEME_DARK only.
	 *
	 * \warning Do not set this variable manually, it won't have any effect.
	 */
	enum mlk_window_theme theme_effective;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \enum mlk_window_cursor
 * \brief Window mouse cursor
 * \note Not all cursors are supported.
 */
enum mlk_window_cursor {
	/**
	 * Standard arrow.
	 */
	MLK_WINDOW_CURSOR_ARROW,

	/**
	 * Text edit cursor like "I".
	 */
	MLK_WINDOW_CURSOR_EDIT,

	/**
	 * Busy cursor.
	 */
	MLK_WINDOW_CURSOR_WAIT,

	/**
	 * Cross-hair for selection.
	 */
	MLK_WINDOW_CURSOR_CROSSHAIR,

	/**
	 * Resizing or moving cursor.
	 */
	MLK_WINDOW_CURSOR_SIZE,

	/**
	 * Forbidden action.
	 */
	MLK_WINDOW_CURSOR_NO,

	/**
	 * Hand, usually for hyper links.
	 */
	MLK_WINDOW_CURSOR_HAND,

	/**
	 * Disable cursor entirely.
	 */
	MLK_WINDOW_CURSOR_OFF,

	/**
	 * Unused sentinel value.
	 */
	MLK_WINDOW_CURSOR_LAST
};

/**
 * Global readonly window info.
 */
extern struct mlk_window mlk_window;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Open the window.
 *
 * \pre title != NULL
 * \pre w > 0
 * \pre h > 0
 * \param title the window title
 * \param w the window width
 * \param h the window height
 * \return 0 on success or -1 on error
 */
int
mlk_window_open(const char *title, unsigned int w, unsigned int h);

/**
 * Change the window mouse cursor.
 *
 * \param cursor the new cursor
 */
void
mlk_window_set_cursor(enum mlk_window_cursor cursor);

/**
 * Select a preferred user theme for the window (auto, dark or light).
 *
 * This function will adjust ::mlk_window::theme_user and
 * ::mlk_window::theme_effective.
 *
 * \param theme the new user theme
 */
void
mlk_window_set_theme(enum mlk_window_theme theme);

/**
 * Destroy the window.
 */
void
mlk_window_finish(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_WINDOW_H */

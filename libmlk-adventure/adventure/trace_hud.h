/*
 * trace_hud.h -- on screen hud
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

#ifndef MOLKO_MOLKO_TRACE_HUD_H
#define MOLKO_MOLKO_TRACE_HUD_H

/**
 * \file trace_hud.h
 * \brief On screen hud
 */

struct theme;

/**
 * \brief Default time to remove messages.
 */
#define TRACE_HUD_TIMEOUT_DEFAULT (3000)

/**
 * \brief Trace HUD options.
 */
struct trace_hud {
	struct theme *theme;	/*!< (+&?) Theme to use. */
	unsigned int timeout;   /*!< (+) Timeout to remove messages. */
};

/**
 * Global trace HUD options.
 */
extern struct trace_hud trace_hud;

/**
 * Handler to use as \ref trace_handler.
 *
 * \pre str != NULL
 * \param str the line to store
 */
void
trace_hud_handler(const char *str);

/**
 * Update the HUD.
 *
 * \param ticks elapsed milliseconds since last frame.
 */
void
trace_hud_update(unsigned int ticks);

/**
 * Draw the HUD.
 */
void
trace_hud_draw(void);

/**
 * Clear the hud.
 */
void
trace_hud_clear(void);

#endif /* !MOLKO_MOLKO_TRACE_HUD_H */

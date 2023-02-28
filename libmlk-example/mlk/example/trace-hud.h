/*
 * trace_hud.h -- on screen hud
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

#ifndef EXAMPLES_TRACE_TRACE_HUD_H
#define EXAMPLES_TRACE_TRACE_HUD_H

#define TRACE_HUD_TIMEOUT_DEFAULT (3000)

struct action;
struct mlk_theme;

struct trace_hud {
	struct mlk_theme *theme;
	unsigned int timeout;
};

extern struct trace_hud trace_hud;

void
trace_hud_handler(const char *);

void
trace_hud_update(unsigned int);

void
trace_hud_draw(void);

void
trace_hud_clear(void);

struct mlk_action *
trace_hud_action(void);

#endif /* !EXAMPLES_TRACE_TRACE_HUD_H */

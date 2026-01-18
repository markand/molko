/*
 * app.h -- main application file
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

#ifndef MLK_EXAMPLE_COROUTINES_APP_H
#define MLK_EXAMPLE_COROUTINES_APP_H

enum mlk_key;
struct mlk_game_ops;

void
app_switch(const struct mlk_game_ops *ops);

/**
 * Wait until a new key is pressed and store it in `*key`.
 */
void
app_key_pressed(enum mlk_key *key);

/**
 * Exactly the opposite as ::app_key_pressed.
 */
void
app_key_released(enum mlk_key *key);

/**
 * Wait until the mouse moved somewhere.
 */
void
app_mouse_moved(int *x, int *y);

void
app_run(void);

extern const struct mlk_game_ops app_game_ops;

#endif /* !MLK_EXAMPLE_COROUTINES_APP_H */

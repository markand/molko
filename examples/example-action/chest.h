/*
 * chest.h -- animated chest
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

#ifndef MLK_EXAMPLE_ACTION_CHEST_H
#define MLK_EXAMPLE_ACTION_CHEST_H

#include <mlk/core/action.h>
#include <mlk/core/animation.h>
#include <mlk/core/drawable.h>
#include <mlk/core/util.h>

#define CHEST(Ptr, Field) \
        (MLK_UTIL_CONTAINER_OF(Ptr, struct chest, Field))

enum chest_state {
	CHEST_STATE_CLOSED,
	CHEST_STATE_OPENING,
	CHEST_STATE_OPENED
};

/**
 * \brief Clickable chest
 *
 * This chest instanciate an action and drawable that user can place on the game
 * loop to render and handle.
 */
struct chest {
	/* read-write */
	int x;
	int y;

	/* read-only */
	enum chest_state state;
	struct mlk_animation animation;
	struct mlk_action action;
	struct mlk_drawable drawable;
};

void
chest_pressed(const struct chest *chest);

void
chest_opened(const struct chest *chest);

void
chest_restart(struct chest *chest);

#endif /* !MLK_EXAMPLE_ACTION_CHEST_H */

/*
 * gamepad.c -- game controller support
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

#include <assert.h>
#include <string.h>

#include <SDL.h>

#include "err.h"
#include "gamepad.h"

int
mlk_gamepad_open(struct mlk_gamepad *pad, int idx)
{
	assert(pad);

	memset(pad, 0, sizeof (*pad));

	if (!(pad->handle = SDL_OpenGamepad(idx)))
		return mlk_errf("%s", SDL_GetError());

	return 0;
}

void
mlk_gamepad_finish(struct mlk_gamepad *pad)
{
	assert(pad);

	if (pad->handle)
		SDL_CloseGamepad(pad->handle);

	memset(pad, 0, sizeof (*pad));
}

#if 0

int
mlk_gamepad_iter_begin(struct mlk_gamepad_iter *it)
{
	assert(it);

	memset(it, 0, sizeof (*it));
	it->index = -1;

	if ((it->end = SDL_NumJoysticks()) < 0) {
		it->end = 0;
		return mlk_errf("%s", SDL_GetError());
	}

	return 0;
}

int
mlk_gamepad_iter_next(struct mlk_gamepad_iter *it)
{
	/*
	 * Go to the next gamepad, we need to iterate because SDL can combines
	 * joystick and game controllers with the same API.
	 */
	for (++it->index; it->index < it->end && !SDL_IsGameController(it->index); ++it->index)
		continue;

	/* End of iteration. */
	if (it->index >= it->end) {
		memset(it, 0, sizeof (*it));
		return 0;
	}

	it->name = SDL_GameControllerNameForIndex(it->index);

	return 1;
}

#endif

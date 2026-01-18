/*
 * app.c -- main application file
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

#include <assert.h>

#include <utlist.h>

#include <mlk/core/coro.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>

#include "app.h"
#include "state-splash.h"

/*
 * Those are coroutines waiting for individual event.
 */
static struct mlk_coro *key_pressed;
static struct mlk_coro *key_released;
static struct mlk_coro *mouse_moved;

static const struct mlk_game_ops *app_state;

static inline void
resume(struct mlk_coro **coro, const void *data, size_t size)
{
	struct mlk_coro *save = *coro;

	if (!save)
		return;

	/* Invalidate so that application code can call immediately after. */
	*coro = NULL;

	/* Queue data without any check. */
	mlk_coro_resume(save, data, size);
}

static inline void
await(struct mlk_coro **coro, void *data, size_t size)
{
	*coro = mlk_coro_self();

	mlk_coro_wait(data, size);
}

static void
app_start(void)
{
	app_switch(&state_splash);
}

static void
app_handle(const union mlk_event *event)
{
	switch (event->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	case MLK_EVENT_KEYDOWN:
		resume(&key_pressed, &event->key, sizeof (event->key));
		break;
	case MLK_EVENT_KEYUP:
		resume(&key_released, &event->key, sizeof (event->key));
		break;
	case MLK_EVENT_MOUSE:
		resume(&mouse_moved, &event->mouse, sizeof (event->mouse));
		break;
	default:
		break;
	}

	if (app_state->handle)
		app_state->handle(event);
}

static void
app_update(unsigned int ticks)
{
	app_state->update(ticks);
}

static void
app_draw(void)
{
	app_state->draw();
}

void
app_switch(const struct mlk_game_ops *ops)
{
	assert(ops);

	app_state = ops;

	/* Detach all waiting coroutines. */
	key_pressed  = NULL;
	key_released = NULL;
	mouse_moved  = NULL;

	if (app_state->start)
		app_state->start();
}

void
app_key_pressed(enum mlk_key *key)
{
	struct mlk_event_key event;

	await(&key_pressed, &event, sizeof (event));
	*key = event.key;
}

void
app_key_released(enum mlk_key *key)
{
	struct mlk_event_key event;

	await(&key_released, &event, sizeof (event));
	*key = event.key;
}

void
app_mouse_moved(int *x, int *y)
{
	struct mlk_event_mouse event;

	await(&mouse_moved, &event, sizeof (event));

	*x = event.x;
	*y = event.y;
}

static void
app_loop(struct mlk_coro *)
{
	mlk_game_loop();
}

static struct mlk_coro app_coro = {
	.name       = "app.loop",
	.entry      = app_loop,
	.stack_size = 2097152
};

void
app_run(void)
{
	mlk_game_init(&app_game_ops);
	mlk_coro_spawn(&app_coro);
}

const struct mlk_game_ops app_game_ops = {
	.start  = app_start,
	.handle = app_handle,
	.update = app_update,
	.draw   = app_draw
};

/*
 * game.c -- main game object
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

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "game.h"
#include "state.h"

struct game game = {
	.state = NULL,
	.state_next = NULL
};

static struct action *
find_empty_action(void)
{
	static struct action null;

	for (size_t i = 0; i < GAME_ACTIONS_MAX; ++i)
		if (memcmp(&game.actions[i], &null, sizeof (struct action)) == 0)
			return &game.actions[i];

	return NULL;
}

static void
clear_actions(void)
{
	for (size_t i = 0; i < GAME_ACTIONS_MAX; ++i) {
		struct action *a = &game.actions[i];

		/* These actions are removed on state change. */
		if (a->flags & ACTION_AUTO_LEAVE) {
			if (a->finish)
				a->finish(a);

			memset(a, 0, sizeof (struct action));
		}
	}
}

static void
handle_actions(const union event *event)
{
	for (size_t i = 0; i < GAME_ACTIONS_MAX; ++i)
		if (game.actions[i].handle)
			game.actions[i].handle(&game.actions[i], event);
}

static void
update_actions(unsigned int ticks)
{
	for (size_t i = 0; i < GAME_ACTIONS_MAX; ++i) {
		struct action *a = &game.actions[i];

		if (!a->update)
			continue;

		if (a->update(a, ticks)) {
			if (a->finish)
				a->finish(a);

			memset(&game.actions[i], 0, sizeof (struct action));
		}
	}
}

static void
draw_actions(void)
{
	for (size_t i = 0; i < GAME_ACTIONS_MAX; ++i)
		if (game.actions[i].draw)
			game.actions[i].draw(&game.actions[i]);
}


void
game_switch(struct state *state, bool quick)
{
	assert(state);

	if (quick) {
		game.state = state;
		game.state->enter();
	} else
		game.state_next = state;
}

void
game_handle(const union event *event)
{
	assert(event);

	if (game.state)
		game.state->handle(event);

	handle_actions(event);
}

void
game_update(unsigned int ticks)
{
	/* Change state if any. */
	if (game.state_next) {
		/* Inform the current state we're gonna leave it. */
		if (game.state)
			game.state->leave();

		game.state = game.state_next;
		game.state->enter();
		game.state_next = NULL;

		/* Remove any actions that must be deleted. */
		clear_actions();
	}

	if (game.state)
		game.state->update(ticks);

	update_actions(ticks);
}

void
game_draw(void)
{
	if (game.state)
		game.state->draw();

	draw_actions();
}

void
game_add_action(const struct action *action)
{
	assert(action);

	struct action *pos;

	if ((pos = find_empty_action()))
		memcpy(pos, action, sizeof (struct action));
}

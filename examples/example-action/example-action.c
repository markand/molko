/*
 * example-action.c -- example on how to use automatic actions
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

#include <string.h>

#include <mlk/core/action-script.h>
#include <mlk/core/action-stack.h>
#include <mlk/core/action.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sprite.h>
#include <mlk/core/state.h>
#include <mlk/core/texture.h>
#include <mlk/core/trace.h>
#include <mlk/core/util.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

#include <mlk/ui/align.h>
#include <mlk/ui/label.h>

#include <mlk/rpg/message.h>

#include "chest.h"
#include "dialog.h"

static struct mlk_state *states[8];

/*
 * Those are two chests shown on the script that the user can click. They will
 * fill up the script scene.
 *
 * For convenience, we arrange those two chests into a mlk_action_stack to
 * avoid calling all mlk_action functions for every chests.
 */

static struct chest chests[2];

/*
 * This structure is the dialog that will be pushed into the mlk_action_script
 * to create a sequence of message the user has to complete.
 */

static struct mlk_action *chests_stack_actions[16];
static struct mlk_action_stack chests_stack = {
	.actions = chests_stack_actions,
	.actionsz = MLK_UTIL_SIZE(chests_stack_actions)
};

/*
 * Just a label to explain to the user what to do.
 */
static struct mlk_label label = {
	.text = "Select your destiny."
};

static void script_left_response(struct dialog *, unsigned int);
static void script_right_response(struct dialog *, unsigned int);

/* {{{ left chest script */

static struct dialog script_left[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEIN,
			.linesz = 1,
			.lines = (const char *[]) {
				"Welcome to this game."
			}
		}
	},
	{
		.msg = {
			.linesz = 3,
			.lines = (const char *[]) {
				"Your journey will begin in this dangerous world,",
				"you must take this before going any further",
				"Are you ready?"
			}
		}
	},
	{
		.response = script_left_response,
		.msg = {
			.flags = MESSAGE_FLAGS_QUESTION,
			.linesz = 2,
			.lines = (const char *[]) {
				"Of course I am",
				"No I'm not sure"
			}
		}
	}
};

static struct dialog script_left_responses[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				 "Don't be so confident"
			}
		},
	},
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				 "Nevermind, I'll do it myself"
			}
		}
	}
};

/* }}} */

/* {{{ right chest script */

static struct dialog script_right[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEIN,
			.linesz = 1,
			.lines = (const char *[]) {
				"Why did you select this chest?"
			}
		}
	},
	{
		.response = script_right_response,
		.msg = {
			.flags = MESSAGE_FLAGS_QUESTION,
			.linesz = 2,
			.lines = (const char *[]) {
				"Because I think there was some gold",
				"I have no freaking idea"
			}
		}
	}
};

static struct dialog script_right_responses[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				"Go away!"
			}
		},
	},
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				"Install OpenBSD then"
			}
		}
	}
};

/* }}} */

/*
 * This is the main script that the loop will execute, we will initialize it
 * depending on which chest the player clicks.
 */
static struct mlk_action *script_actions[16];
static struct mlk_action_script script;

static void
script_left_response(struct dialog *dlg, unsigned int index)
{
	(void)dlg;

	mlk_action_script_append(&script, dialog_init(&script_left_responses[index]));
}

static void
script_right_response(struct dialog *dlg, unsigned int index)
{
	(void)dlg;

	mlk_action_script_append(&script, dialog_init(&script_right_responses[index]));
}

static void
script_init(struct dialog *msgs, size_t msgsz)
{
	int err;
	struct mlk_action *action;

	mlk_action_script_init(&script, script_actions, MLK_UTIL_SIZE(script_actions));

	for (size_t i = 0; i < msgsz; ++i) {
		action = dialog_init(&msgs[i]);

		if ((err = mlk_action_script_append(&script, action)) < 0)
			mlk_panicf("%s", mlk_err_string(err));
	}

	mlk_action_script_start(&script);
}

static void
script_left_run(struct chest *chest)
{
	(void)chest;

	script_init(script_left, MLK_UTIL_SIZE(script_left));
}

static void
script_right_run(struct chest *chest)
{
	(void)chest;

	script_init(script_right, MLK_UTIL_SIZE(script_right));
}

static void
chests_init(void)
{
	/*
	 * Chest #0 is at center 1st half of the screen.
	 * Chest #1 is at center of 2nd half of the sreen.
	 */
	const unsigned int cw = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellw;
	const unsigned int ch = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellh;

	mlk_align(MLK_ALIGN_CENTER, &chests[0].x, &chests[0].y, cw, ch,
	    0, 0, MLK_EXAMPLE_W / 2, MLK_EXAMPLE_H);
	mlk_align(MLK_ALIGN_CENTER, &chests[1].x, &chests[1].y, cw, ch,
	    MLK_EXAMPLE_W / 2, 0, MLK_EXAMPLE_W / 2, MLK_EXAMPLE_H);

	chests[0].run = script_left_run;
	chests[1].run = script_right_run;

	mlk_action_stack_add(&chests_stack, chest_init(&chests[0]));
	mlk_action_stack_add(&chests_stack, chest_init(&chests[1]));
}

static void
label_init(void)
{
	unsigned int lw, lh;

	mlk_label_query(&label, &lw, &lh);
	mlk_align(MLK_ALIGN_CENTER, &label.x, &label.y, lw, lh,
	    0, 0, MLK_EXAMPLE_W, MLK_EXAMPLE_H / 2);
}

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-action")) < 0)
		mlk_panicf("mlk_example_init: %s", mlk_err_string(err));

	chests_init();
	label_init();
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		/*
		 * The main script is "modal", we must not update anything else than
		 * it until it's complete.
		 */
		if (mlk_action_script_completed(&script))
			mlk_action_stack_handle(&chests_stack, ev);
		else
			mlk_action_script_handle(&script, ev);
		break;
	}
}

static void
update(struct mlk_state *st, unsigned int ticks)
{
	(void)st;

	mlk_action_stack_update(&chests_stack, ticks);

	if (mlk_action_script_update(&script, ticks))
		mlk_action_script_finish(&script);
}

static void
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&label);
	mlk_action_stack_draw(&chests_stack);

	/*
	 * Always draw the script in the end in case it overlaps everything
	 * else.
	 */
	mlk_action_script_draw(&script);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init(states, MLK_UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();
}

static void
quit(void)
{
	mlk_example_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	quit();
}

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

/*
 * This example uses the coroutine library to create a more sequential sequence
 * of actions.
 */

#include <string.h>

#include <mlk/core/action-script.h>
#include <mlk/core/action-stack.h>
#include <mlk/core/action.h>
#include <mlk/core/coro.h>
#include <mlk/core/drawable-stack.h>
#include <mlk/core/drawable.h>
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

/*
 * Just a label to explain to the user what to do.
 */
static struct mlk_label label = {
	.text = "Select your destiny."
};

/* {{{ right chest script */

#if 0
static struct dialog script_right[] = {
	{
		.msg = {
			.flags = MLK_MESSAGE_FLAGS_FADEIN,
			.linesz = 1,
			.lines = (const char *[]) {
				"Why did you select this chest?"
			}
		}
	},
	{
		.response = script_right_response,
		.msg = {
			.selectable = 0x3,
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
			.flags = MLK_MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				"Go away!"
			}
		},
	},
	{
		.msg = {
			.flags = MLK_MESSAGE_FLAGS_FADEOUT,
			.linesz = 1,
			.lines = (const char *[]) {
				"Install OpenBSD then"
			}
		}
	}
};
#endif

/* }}} */

static struct mlk_action_stack action_stack = MLK_ACTION_STACK_DECL(32);
static struct mlk_drawable_stack drawable_stack = MLK_DRAWABLE_STACK_DECL(32);

/*
 * Coroutine helpers to show messages.
 */

static size_t
show(struct dialog *dialog,
     enum mlk_message_flags flags,
     size_t selectable,
     size_t linesz,
     const char * const *lines)
{
	dialog->msg.flags      = flags;
	dialog->msg.selectable = selectable;
	dialog->msg.linesz     = linesz;
	dialog->msg.lines      = lines;

	/* Register dialog and wait for it to complete. */
	dialog_restart(dialog);
	mlk_action_stack_add(&action_stack, &dialog->action);
	dialog_completed(dialog);

	return dialog->msg.selected;
}

static inline void
message(struct dialog *dialog,
        enum mlk_message_flags flags,
        size_t linesz,
        const char * const *lines)
{
	show(dialog, flags, 0, linesz, lines);
}

static inline size_t
prompt(struct dialog *dialog,
       size_t selectable,
       size_t linesz,
       const char * const *lines)
{
	return show(dialog, 0, selectable, linesz, lines);
}

static struct {
	struct mlk_coro coro;
	struct chest chest;
	struct dialog dialog;
} sequence;

/*
 * This is a coroutine.
 *
 * Most of the function you read here are yielding until their condition
 * evaluate to true. Ending with a very clean and sequential approach without
 * blocking the main loop.
 */
static void
sequence_entry(struct mlk_coro *)
{
	const char *final[1];
	int choice;

	/* Wait for chest to be clicked. */
	chest_pressed(&sequence.chest);

	/* Wait for chest animation to complete. */
	chest_opened(&sequence.chest);

	message(&sequence.dialog, MLK_MESSAGE_FLAGS_FADEIN, 1, (const char *[]) {
		"Welcome to this game."
	});

	message(&sequence.dialog, 0, 3, (const char *[]) {
		"Your journey will begin in this dangerous world,",
		"you must take this before going any further",
		"Are you ready?"
	});

	choice = prompt(&sequence.dialog, 0x03, 2, (const char *[]) {
		"Of course I am",
		"No I'm not sure"
	});

	if (choice)
		final[0] = "Nevermind, I'll do it myself";
	else
		final[0] = "Don't be so confident";

	message(&sequence.dialog, MLK_MESSAGE_FLAGS_FADEOUT, 1, final);
}

static void
sequence_init(void)
{
	const unsigned int cw = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellw;
	const unsigned int ch = mlk_registry_sprites[MLK_REGISTRY_TEXTURE_CHEST].cellh;
	struct chest *chest = &sequence.chest;

	/* Initialize chest and add it to the action stack. */
	chest_restart(chest);

	mlk_action_stack_add(&action_stack, &chest->action);
	mlk_drawable_stack_add(&drawable_stack, &chest->drawable);

	mlk_align(MLK_ALIGN_CENTER, &chest->x, &chest->y, cw, ch,
	    0, 0, MLK_EXAMPLE_W, MLK_EXAMPLE_H);

	/* Create the coroutine. */
	sequence.coro.entry = sequence_entry;
	mlk_coro_spawn(&sequence.coro);
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
	if (mlk_example_init("example-action") < 0)
		mlk_panic();

	label_init();
	sequence_init();
}

static void
handle(struct mlk_state *, const union mlk_event *ev)
{
	switch (ev->type) {
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		mlk_action_stack_handle(&action_stack, ev);
		break;
	}
}

static void
update(struct mlk_state *, unsigned int ticks)
{
	mlk_action_stack_update(&action_stack, ticks);
	mlk_drawable_stack_update(&drawable_stack, ticks);

	if (mlk_coro_resumable(&sequence.coro))
		mlk_coro_resume(&sequence.coro);
}

static void
draw(struct mlk_state *)
{
	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&label);
	mlk_action_stack_draw(&action_stack);
	mlk_drawable_stack_draw(&drawable_stack);
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

	mlk_game_init();
	mlk_game_loop(&state);
}

static void
quit(void)
{
	mlk_example_finish();
}

int
main(int, char **)
{
	init();
	run();
	quit();
}

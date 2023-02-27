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
#include <mlk/core/maths.h>
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

/* Message width is 80% of window width and height is auto computed. */
#define QMW (MLK_EXAMPLE_W * 0.8)

/* Position of each message. */
#define QMX ((MLK_EXAMPLE_W - QMW) / 2)
#define QMY (MLK_EXAMPLE_H * 0.2)

/* Fading time and spacing. */
#define QMD 250
#define QMS 10

static struct mlk_state *states[8];

/*
 * Those are two chests shown on the script that the user can click. They will
 * fill up the script scene.
 *
 * For convenience, we arrange those two chests into a mlk_action_stack to
 * avoid calling all mlk_action functions for every chests.
 */
static void chest_handle(struct mlk_action *, const union mlk_event *);
static void chest_draw(struct mlk_action *);

static struct chest {
	int x;
	int y;
	int open;
	struct mlk_sprite *sprite;
	struct mlk_action action;
} chests[2] = {
	[0] = {
		.sprite = &registry_sprites[REGISTRY_TEXTURE_CHEST],
		.action = {
			.data = &chests[0],
			.draw = chest_draw,
			.handle = chest_handle,
		}
	},
	[1] = {
		.sprite = &registry_sprites[REGISTRY_TEXTURE_CHEST],
		.action = {
			.data = &chests[1],
			.draw = chest_draw,
			.handle = chest_handle,
		}
	}
};

/*
 * This structure is the item that will be pushed into the mlk_action_script
 * to create a sequence of message the user has to complete.
 */
struct item {
	struct message msg;
	struct mlk_action act;
};

static struct mlk_action *chests_stack_actions[16] = {
	[0] = &chests[0].action,
	[1] = &chests[1].action
};
static struct mlk_action_stack chests_stack = {
	.actions = chests_stack_actions,
	.actionsz = MLK_UTIL_SIZE(chests_stack_actions)
};

/*
 * Just a label to explain to the user what to do.
 */
static struct label label = {
	.flags = LABEL_FLAGS_SHADOW,
	.text = "Select your destiny."
};

static void     item_handle(struct mlk_action *, const union mlk_event *);
static int      item_update(struct mlk_action *, unsigned int);
static void     item_draw(struct mlk_action *);

static void     script_init_quest1(void);
static void     script_init_quest2(void);

static void     response_quest1_start(struct mlk_action *);
static void     response_quest2_start(struct mlk_action *);

static struct item quest1[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEIN,
			.linesz = 1,
			.lines = (const char *[]) {
				"Welcome to this game."
			}
		},
		.act = {
			.data = &quest1[0].msg,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw
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
		},
		.act = {
			.data = &quest1[1].msg,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw
		}
	},
	{
		.msg = {
			.flags = MESSAGE_FLAGS_QUESTION,
			.linesz = 2,
			.lines = (const char *[]) {
				"Of course I am",
				"No I'm not sure"
			}
		},
		.act = {
			.data = &quest1[2].msg,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw,
		}
	},
	/* Text will be replaced on start depending on the question. */
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT
		},
		.act = {
			.data = &quest1[3].msg,
			.start = response_quest1_start,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw
		}
	}
};

static struct item quest2[] = {
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEIN,
			.linesz = 1,
			.lines = (const char *[]) {
				"Why did you select this chest?"
			}
		},
		.act = {
			.data = &quest2[0].msg,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw
		}
	},
	{
		.msg = {
			.flags = MESSAGE_FLAGS_QUESTION,
			.linesz = 2,
			.lines = (const char *[]) {
				"Because I think there was some gold",
				"I have no freaking idea"
			}
		},
		.act = {
			.data = &quest2[1].msg,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw,
		}
	},

	/* See comment in quest 1. */
	{
		.msg = {
			.flags = MESSAGE_FLAGS_FADEOUT
		},
		.act = {
			.data = &quest2[2].msg,
			.start = response_quest2_start,
			.handle = item_handle,
			.update = item_update,
			.draw = item_draw
		}
	}
};

/*
 * This is the main script that the loop will execute, we will initialize it
 * depending on which chest the player clicks.
 */
static struct mlk_action *script_actions[16];
static struct mlk_action_script script;

static void
chest_handle(struct mlk_action *act, const union mlk_event *ev)
{
	struct chest *chest = act->data;
	unsigned int cw, ch;

	/* Make sure that we don't operate on a already opened chest. */
	if (chest->open)
		return;

	/*
	 * We are only interested if the event is a click on the chest itself
	 * so we have to test because actions have no notion of geometry.
	 */
	if (ev->type != MLK_EVENT_CLICKDOWN)
		return;

	cw = registry_sprites[REGISTRY_TEXTURE_CHEST].cellw;
	ch = registry_sprites[REGISTRY_TEXTURE_CHEST].cellh;

	if (!mlk_maths_is_boxed(chest->x, chest->y, cw, ch, ev->click.x, ev->click.y))
		return;

	/* Also, make sure that we don't operate on a already opened chest. */
	chest->open = 1;

	/*
	 * Now depending on the chest, update the main script with actual code
	 * we're interested in.
	 */
	memset(script_actions, 0, sizeof (script_actions));
	mlk_action_script_init(&script, script_actions, MLK_UTIL_SIZE(script_actions));

	if (chest == &chests[0])
		script_init_quest1();
	else
		script_init_quest2();
}

static void
chest_draw(struct mlk_action *act)
{
	const struct chest *chest = act->data;
	int column;

	if (chest->open)
		column = 3;
	else
		column = 0;

	mlk_sprite_draw(chest->sprite, 0, column, chest->x, chest->y);
}

static void
item_handle(struct mlk_action *act, const union mlk_event *ev)
{
	message_handle(act->data, ev);
}

static int
item_update(struct mlk_action *act, unsigned int ticks)
{
	return message_update(act->data, ticks);
}

static void
item_draw(struct mlk_action *act)
{
	message_draw(act->data);
}

static void
script_init_quest(struct item *m, size_t n)
{
	int err;

	for (size_t i = 0; i < n; ++i) {
		m[i].msg.x = QMX;
		m[i].msg.y = QMY;
		m[i].msg.w = QMW;
		m[i].msg.delay = QMD;
		m[i].msg.spacing = QMS;

		message_start(&m[i].msg);
		message_query(&m[i].msg, NULL, &m[i].msg.h);

		if ((err = mlk_action_script_append(&script, &m[i].act)) < 0)
			mlk_panicf("%s", mlk_err_string(err));
	}

	mlk_action_script_start(&script);
}

static void
script_init_quest1(void)
{
	script_init_quest(quest1, MLK_UTIL_SIZE(quest1));
}

static void
script_init_quest2(void)
{
	script_init_quest(quest2, MLK_UTIL_SIZE(quest2));
}

static void
response_quest1_start(struct mlk_action *act)
{
	static const char *line;

	struct message *question = &quest1[2].msg;
	struct message *cur = act->data;

	cur->linesz = 1;
	cur->lines = &line;

	if (question->index == 0)
		 line = "Don't be so confident";
	else
		 line = "Nevermind, I'll do it myself";

	message_query(cur, NULL, &cur->h);
}

static void
response_quest2_start(struct mlk_action *act)
{
	static const char *line;

	struct message *question = &quest2[1].msg;
	struct message *cur = act->data;

	cur->linesz = 1;
	cur->lines = &line;

	if (question->index == 0)
		line = "Go away!";
	else
		line = "Install OpenBSD then";

	message_query(cur, NULL, &cur->h);
}

static void
chests_init(void)
{
	/*
	 * Chest #0 is at center 1st half of the screen.
	 * Chest #1 is at center of 2nd half of the sreen.
	 */
	const unsigned int cw = registry_sprites[REGISTRY_TEXTURE_CHEST].cellw;
	const unsigned int ch = registry_sprites[REGISTRY_TEXTURE_CHEST].cellh;

	align(ALIGN_CENTER, &chests[0].x, &chests[0].y, cw, ch,
	    0, 0, MLK_EXAMPLE_W / 2, MLK_EXAMPLE_H);
	align(ALIGN_CENTER, &chests[1].x, &chests[1].y, cw, ch,
	    MLK_EXAMPLE_W / 2, 0, MLK_EXAMPLE_W / 2, MLK_EXAMPLE_H);
}

static void
label_init(void)
{
	unsigned int lw, lh;

	label_query(&label, &lw, &lh);
	align(ALIGN_CENTER, &label.x, &label.y, lw, lh,
	    0, 0, MLK_EXAMPLE_W, MLK_EXAMPLE_H / 2);
}

static void
init(void)
{
	int err;

	if ((err = mlk_example_init("example-action")) < 0)
		mlk_panicf("example_init: %s", mlk_err_string(err));

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

	mlk_painter_set_color(0x4f8fbaff);
	mlk_painter_clear();
	label_draw(&label);
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

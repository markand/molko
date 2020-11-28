/*
 * example-action.c -- example on how to use automatic actions
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

#include <core/action.h>
#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/image.h>
#include <core/maths.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/script.h>
#include <core/sprite.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/theme.h>
#include <ui/ui.h>

#include <rpg/message.h>
#include <rpg/rpg.h>

#include <assets/sprites/chest.h>
#include <assets/sprites/people.h>

#define W       (1280)
#define H       (720)

#define MW      (W * 0.75)
#define MX      ((W / 2) - (MW / 2))
#define MY      (100)

/* This is a stack of "parallel" events. */
static struct action_stack events;

/* This is a stack of modal events. */
static struct action_stack modal;

/*
 * Event object for the chest to click on.
 */
static struct {
	struct message msg;
	struct action msg_act;
	int x;
	int y;
	bool opened;
	struct texture image;
	struct sprite sprite;
	struct action event;
} chest = {
	.msg = {
		.x = MX,
		.y = MY,
		.w = MW,
		.text = {
			"100000 pièces.",
			"Te voilà riche sale file de crevette."
		}
	}
};

/*
 * Event object for the guide who ask dialog with you.
 *
 * It is a script spawned upon click like this.
 *
 * [guide.event]
 *       |           spawn
 *   (on click)------------------->[message 1]
 *                                      v
 *                                 [message 2]
 *                                      v
 *                                 [question 3]
 *                   spawn              v
 * [response]<---------------------[check result]
 *     v
 * [message 4/5]
 */
static struct {
	struct {
		struct message msg;
		struct action act;
	} msgs[5];

	/* This is the sprite and action to click on. */
	int x;
	int y;
	struct texture image;
	struct sprite sprite;
	struct action event;

	/* This is the event for the response. */
	struct action response;

	struct script script;
	struct action script_act;
} guide = {
	.msgs = {
		{
			.msg = {
				.x = MX,
				.y = MY,
				.w = MW,
				.delay = MESSAGE_DELAY_DEFAULT,
				.flags = MESSAGE_FLAGS_FADEIN,
				.text = {
					"Bienvenue dans ce monde Molko."
				}
			},
		},
		{
			.msg = {
				.x = MX,
				.y = MY,
				.w = MW,
				.text = {
					"Penses tu vraiment pouvoir me battre ?"
				}
			}
		},
		{
			.msg = {
				.x = MX,
				.y = MY,
				.w = MW,
				.flags = MESSAGE_FLAGS_QUESTION,
				.text = {
					"Non j'ai la trouille.",
					"Bien sûr, je suis la légende."
				}
			}
		},

		/* In case of NO. */
		{
			.msg = {
				.x = MX,
				.y = MY,
				.w = MW,
				.text = {
					"Poule mouillée."
				}
			}
		},

		/* In case of YES. */
		{
			.msg = {
				.x = MX,
				.y = MY,
				.w = MW,
				.text = {
					"Prépare toi à souffrir."
				}
			}
		}
	}
};

static bool
guide_response_update(struct action *act, unsigned int ticks)
{
	/* Immediately return to get access to end. */
	(void)act;
	(void)ticks;

	return true;
}

static void
guide_response_end(struct action *act)
{
	(void)act;

	/* We add a final response depending on the result. */
	const int index = guide.msgs[2].msg.index + 3;

	message_action(&guide.msgs[index].msg, &guide.msgs[index].act);
	message_query(&guide.msgs[index].msg, NULL, &guide.msgs[index].msg.h);
	action_stack_add(&modal, &guide.msgs[index].act);
}

static void
guide_popup(void)
{
	/* Prepare the script with first 3 messages. */
	for (size_t i = 0; i < 3; ++i) {
		message_action(&guide.msgs[i].msg, &guide.msgs[i].act);
		message_query(&guide.msgs[i].msg, NULL, &guide.msgs[i].msg.h);
		script_append(&guide.script, &guide.msgs[i].act);
	}

	/* Reset the message index. */
	guide.msgs[2].msg.index = 0;

	/* This is final action that analyze user input. */
	guide.response.update = guide_response_update;
	guide.response.end = guide_response_end;
	script_append(&guide.script, &guide.response);

	script_action(&guide.script, &guide.script_act);
	action_stack_add(&modal, &guide.script_act);
}

static void
guide_handle(struct action *act, const union event *ev)
{
	(void)act;

	if (!action_stack_completed(&modal))
		return;

	switch (ev->type) {
	case EVENT_CLICKDOWN:
		if (maths_is_boxed(guide.x, guide.y,
		    guide.sprite.cellw, guide.sprite.cellh,
		    ev->click.x, ev->click.y))
			guide_popup();
		break;
	default:
		break;
	}
}

static void
guide_draw(struct action *act)
{
	(void)act;

	sprite_draw(&guide.sprite, 0, 0, guide.x, guide.y);
}

static void
guide_init(void)
{
	if (!image_openmem(&guide.image, sprites_people, sizeof (sprites_people)))
		panic();

	sprite_init(&guide.sprite, &guide.image, 48, 48);

	/* This is the global guide action. */
	guide.event.handle = guide_handle;
	guide.event.draw = guide_draw;
	guide.x = 200;
	guide.y = 200;
}

static void
chest_handle(struct action *act, const union event *ev)
{
	(void)act;

	if (chest.opened || !action_stack_completed(&modal))
		return;

	switch (ev->type) {
	case EVENT_CLICKDOWN:
		if (maths_is_boxed(chest.x, chest.y, chest.sprite.cellw, chest.sprite.cellh,
		    ev->click.x, ev->click.y)) {
			chest.opened = true;
			message_action(&chest.msg, &chest.msg_act);
			message_query(&chest.msg, NULL, &chest.msg.h);
			action_stack_add(&modal, &chest.msg_act);
		}
	default:
		break;
	}
}

static void
chest_draw(struct action *act)
{
	(void)act;

	const int row = chest.opened ? 3 : 0;

	sprite_draw(&chest.sprite, row, 0, chest.x, chest.y);
}

static void
chest_init(void)
{
	if (!image_openmem(&chest.image, sprites_chest, sizeof (sprites_chest)))
		panic();

	sprite_init(&chest.sprite, &chest.image, 32, 32);

	chest.x = 100;
	chest.y = 100;
	chest.event.handle = chest_handle;
	chest.event.draw = chest_draw;
}

static void
init(void)
{
	if (!core_init("fr.malikania", "actions") || !ui_init() || !rpg_init())
		panic();
	if (!window_open("Example - Action", W, H))
		panic();

	guide_init();
	chest_init();
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		action_stack_handle(&events, ev);
		action_stack_handle(&modal, ev);
		break;
	}
}

static void
update(struct state *st, unsigned int ticks)
{
	(void)st;

	action_stack_update(&events, ticks);
	action_stack_update(&modal, ticks);
}

static void
draw(struct state *st)
{
	(void)st;

	painter_set_color(0xffffffff);
	painter_clear();
	action_stack_draw(&events);
	action_stack_draw(&modal);
	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	action_stack_add(&events, &chest.event);
	action_stack_add(&events, &guide.event);

	game_switch(&state, true);
	game_loop();
}

static void
quit(void)
{
	window_finish();
	rpg_finish();
	ui_finish();
	core_finish();
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init();
	run();
	quit();

	return 0;
}

/*
 * example-audio.c -- show how to use sounds and music
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

#include <core/core.h>
#include <core/event.h>
#include <core/game.h>
#include <core/music.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/sound.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/label.h>
#include <ui/theme.h>
#include <ui/ui.h>

#include <assets/music/vabsounds-romance.h>
#include <assets/sounds/fire.h>

#define W       1280
#define H       720

static struct state *states[1];
static struct music music;
static struct sound sound;

static struct label label_music = {
	.text = "Music: <Space> play, <p> pause, <r> resume, <q> stop, <l> loop.",
	.x = 10,
	.y = 10,
	.flags = LABEL_FLAGS_SHADOW
};

static struct label label_sound = {
	.text = "Sound: click anywhere to pop a sound.",
	.x = 10,
	.y = 30,
	.flags = LABEL_FLAGS_SHADOW
};

static void
init(void)
{
	if (core_init("fr.malikania", "example-audio") < 0 || ui_init() < 0)
		panic();
	if (window_open("Example - Audio", W, H) < 0)
		panic();
	if (music_openmem(&music, assets_music_vabsounds_romance, sizeof (assets_music_vabsounds_romance)) < 0 ||
	    sound_openmem(&sound, assets_sounds_fire, sizeof (assets_sounds_fire)) < 0)
		panic();
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	core_finish();
}

static void
handle(struct state *st, const union event *ev)
{
	(void)st;

	switch (ev->type) {
	case EVENT_CLICKDOWN:
		if (sound_play(&sound) < 0)
			panic();
		break;
	case EVENT_KEYDOWN:
		switch (ev->key.key) {
		case KEY_p:
			music_pause(&music);
			break;
		case KEY_r:
			music_resume(&music);
			break;
		case KEY_q:
			music_stop(&music);
			break;
		case KEY_l:
			music_play(&music, MUSIC_LOOP);
			break;
		case KEY_SPACE:
			music_play(&music, 0);
			break;
		default:
			break;
		}
		break;
	case EVENT_QUIT:
		game_quit();
		break;
	default:
		break;
	}
}

static void
draw(struct state *st)
{
	(void)st;

	painter_set_color(0x006554ff);
	painter_clear();
	label_draw(&label_music);
	label_draw(&label_sound);
	painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	game_init(states, UTIL_SIZE(states));
	game_push(&state);
	game_loop();

	music_finish(&music);
	sound_finish(&sound);
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

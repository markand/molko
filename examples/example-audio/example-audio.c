/*
 * example-audio.c -- show how to use sounds and music
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

#include <mlk/core/core.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/music.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sound.h>
#include <mlk/core/state.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/label.h>
#include <mlk/ui/theme.h>
#include <mlk/ui/ui.h>

#include <assets/music/vabsounds-romance.h>
#include <assets/sounds/fire.h>

#define W       1280
#define H       720

static struct state *states[1];
static struct mlk_music music;
static struct mlk_sound sound;

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
	if (mlk_core_init("fr.malikania", "example-audio") < 0 || ui_init() < 0)
		mlk_panic();
	if (window_open("Example - Audio", W, H) < 0)
		mlk_panic();
	if (mlk_music_openmem(&music, assets_music_vabsounds_romance, sizeof (assets_music_vabsounds_romance)) < 0 ||
	    mlk_sound_openmem(&sound, assets_sounds_fire, sizeof (assets_sounds_fire)) < 0)
		mlk_panic();
}

static void
quit(void)
{
	window_finish();
	ui_finish();
	mlk_core_finish();
}

static void
handle(struct state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
		if (mlk_sound_play(&sound) < 0)
			mlk_panic();
		break;
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_p:
			mlk_music_pause(&music);
			break;
		case MLK_KEY_r:
			mlk_music_resume(&music);
			break;
		case MLK_KEY_q:
			mlk_music_stop(&music);
			break;
		case MLK_KEY_l:
			mlk_music_play(&music, MLK_MUSIC_LOOP);
			break;
		case MLK_KEY_SPACE:
			mlk_music_play(&music, 0);
			break;
		default:
			break;
		}
		break;
	case MLK_EVENT_QUIT:
		mlk_game_quit();
		break;
	default:
		break;
	}
}

static void
draw(struct state *st)
{
	(void)st;

	mlk_painter_set_color(0x006554ff);
	mlk_painter_clear();
	label_draw(&label_music);
	label_draw(&label_sound);
	mlk_painter_present();
}

static void
run(void)
{
	struct state state = {
		.handle = handle,
		.draw = draw
	};

	mlk_game_init(states, UTIL_SIZE(states));
	mlk_game_push(&state);
	mlk_game_loop();

	mlk_music_finish(&music);
	mlk_sound_finish(&sound);
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

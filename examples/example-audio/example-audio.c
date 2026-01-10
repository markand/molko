/*
 * example-audio.c -- show how to use sounds and music
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
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/registry.h>

static struct mlk_music *music;
static struct mlk_sound *sound;

static struct mlk_label label_music = {
	.text = "Music: <Space> play, <p> pause, <r> resume, <q> stop, <l> loop.",
	.x = 10,
	.y = 10
};

static struct mlk_label label_sound = {
	.text = "Sound: click anywhere to pop a sound.",
	.x = 10,
	.y = 30
};

static void
init(void)
{
	if (mlk_example_init("example-audio") < 0)
		mlk_panic();

	sound = &mlk_registry_sounds[MLK_REGISTRY_SOUND_FIRE];
	music = &mlk_registry_music[MLK_REGISTRY_MUSIC_ROMANCE];
}

static void
quit(void)
{
	mlk_example_finish();
}

static void
handle(struct mlk_state *st, const union mlk_event *ev)
{
	(void)st;

	switch (ev->type) {
	case MLK_EVENT_CLICKDOWN:
		if (mlk_sound_play(sound) < 0)
			mlk_panic();
		break;
	case MLK_EVENT_KEYDOWN:
		switch (ev->key.key) {
		case MLK_KEY_P:
			mlk_music_pause(music);
			break;
		case MLK_KEY_R:
			mlk_music_resume(music);
			break;
		case MLK_KEY_Q:
			mlk_music_stop(music);
			break;
		case MLK_KEY_L:
			mlk_music_play(music, MLK_MUSIC_LOOP);
			break;
		case MLK_KEY_SPACE:
			mlk_music_play(music, 0);
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
draw(struct mlk_state *st)
{
	(void)st;

	mlk_painter_set_color(MLK_EXAMPLE_BG);
	mlk_painter_clear();
	mlk_label_draw(&label_music);
	mlk_label_draw(&label_sound);
	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_state state = {
		.handle = handle,
		.draw = draw
	};

	mlk_game_init();
	mlk_game_loop(&state);

	mlk_music_finish(music);
	mlk_sound_finish(sound);
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

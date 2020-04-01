/*
 * example-sound.c -- show how to use sounds
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

#include <clock.h>
#include <event.h>
#include <label.h>
#include <painter.h>
#include <panic.h>
#include <sound.h>
#include <sys.h>
#include <theme.h>
#include <util.h>
#include <window.h>

/* https://freesound.org/people/VABsounds/sounds/423658 */
#include "assets/sounds/vabsounds-romance.h"

#define W 1920
#define H 1080

static struct sound sound;
static struct label label = {
	.text = "Keys: <s> start, <p> pause, <r> resume, <q> stop, <l> loop",
	.x = 10,
	.y = 10,
	.w = 1920,
	.h = 32,
	.flags = LABEL_NO_HCENTER
};

static void
init(void)
{
	if (!sys_init() ||
	    !window_init("Example - Sound", W, H) ||
	    !theme_init())
		panic();

	if (!sound_openmem(&sound, vabsounds_romance, sizeof (vabsounds_romance)))
		panic();
}

static void
quit(void)
{
	theme_finish();
	window_finish();
	sys_finish();
}

static void
run(void)
{
	struct clock clock = {0};

	clock_start(&clock);

	for (;;) {
		union event ev;
		unsigned int elapsed = clock_elapsed(&clock);

		clock_start(&clock);

		while (event_poll(&ev)) {
			switch (ev.type) {
			case EVENT_KEYDOWN:
				switch (ev.key.key) {
				case KEY_s:
					sound_play(&sound);
					break;
				case KEY_p:
					sound_pause(&sound);
					break;
				case KEY_r:
					sound_resume(&sound);
					break;
				case KEY_q:
					sound_stop(&sound);
					break;
				case KEY_l:
					sound.flags ^= SOUND_LOOP;
					sound_play(&sound);
					break;
				default:
					break;
				}
				break;
			case EVENT_QUIT:
				return;
			default:
				break;
			}
		}

		painter_set_color(0x006554ff);
		painter_clear();
		label_draw(&label);
		painter_present();

		if ((elapsed = clock_elapsed(&clock)) < 20)
			delay(20 - elapsed);
	}
}

int
main(int argc, char **argv)
{
	init();
	run();
	quit();
}

/*
 * splashscreen.c -- splash screen state
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
#include <stdlib.h>
#include <string.h>

#include <core/font.h>
#include <core/game.h>
#include <core/image.h>
#include <core/painter.h>
#include <core/panic.h>
#include <core/state.h>
#include <core/sys.h>
#include <core/texture.h>
#include <core/util.h>
#include <core/window.h>

#include <ui/align.h>

#include <adventure/assets/fonts/cubic.h>

#include "splashscreen.h"
#include "mainmenu.h"

#define DELAY 3000

struct splashscreen {
	struct texture tex;
	int x;
	int y;
	struct state *next;
	unsigned int elapsed;
};

static struct splashscreen *
init(struct state *next)
{
	struct splashscreen *splash;
	struct font font;

	splash = ecalloc(1, sizeof (*splash));
	splash->next = next;

	if (!font_openmem(&font, fonts_cubic, sizeof (fonts_cubic), 80))
		panic();

	font.style = FONT_STYLE_ANTIALIASED;

	if (!font_render(&font, &splash->tex, "malikania", 0x19332dff))
		panic();

	align(ALIGN_CENTER, &splash->x, &splash->y, splash->tex.w, splash->tex.h,
	    0, 0, window.w, window.h);
	font_finish(&font);

	return splash;
}

static void
update(struct state *state, unsigned int ticks)
{
	struct splashscreen *splash = state->data;

	splash->elapsed += ticks;

	if (splash->elapsed >= DELAY)
		game_switch(splash->next, false);
}

static void
draw(struct state *state)
{
	struct splashscreen *splash = state->data;

	painter_set_color(0xffffffff);
	painter_clear();
	texture_draw(&splash->tex, splash->x, splash->y);
}

static void
finish(struct state *state)
{
	struct splashscreen *splash = state->data;

	if (!splash)
		return;

	texture_finish(&splash->tex);

	free(splash);
	memset(state, 0, sizeof (*state));
}

void
splashscreen_state(struct state *state, struct state *next)
{
	assert(state);
	assert(next);

	memset(state, 0, sizeof (*state));
	state->data = init(next);
	state->update = update;
	state->draw = draw;
	state->finish = finish;
}

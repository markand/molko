/*
 * panic_state.c -- panic state
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
#include <stdio.h>
#include <stdlib.h>

#include <error.h>
#include <event.h>
#include <font.h>
#include <game.h>
#include <painter.h>
#include <sys.h>
#include <texture.h>
#include <util.h>
#include <window.h>
#include <map_state.h>

#include "panic_state.h"

#define BACKGROUND 0x4f5070ff
#define FOREGROUND 0xffffffff

#define FONT "fonts/Lato-Regular.ttf"
#define FONT_SZ 16

#define PADDING 20

#define OUT "molko-adventure.txt"

struct label {
	const char *text;
	struct texture *texture;
};

static struct {
	struct font *font;
} data;

static struct label headers[] = {
	{ "An unrecoverable error occured and the game cannot continue.", NULL },
	{ "Please report the detailed error as provided below.", NULL },
};

static struct label bottom[] = {
	{ "Press <s> to save information and generate a core dump.", NULL },
	{ "Press <q> to quit without saving information.", NULL }
};

static struct label lerror;

static void
enter(void)
{
}

static void
dump(void)
{
	FILE *fp;

	if (!(fp = fopen(OUT, "w")))
		goto dump;

	/* Print various information. */
	fprintf(fp, "Molko's Adventure crash dump report\n");
	fprintf(fp, "== state map dump ==\n");
	fprintf(fp, "map:\n");
	fprintf(fp, "  w     %u\n", map_state_data.map.w);
	fprintf(fp, "  h     %u\n", map_state_data.map.h);
	fprintf(fp, "player:\n");
	fprintf(fp, "  x:    %u\n", map_state_data.player.x);
	fprintf(fp, "  y:    %u\n", map_state_data.player.y);
	fprintf(fp, "view:\n");
	fprintf(fp, "  w:    %u\n", map_state_data.view.w);
	fprintf(fp, "  h:    %u\n", map_state_data.view.h);
	fprintf(fp, "  x:    %u\n", map_state_data.view.x);
	fprintf(fp, "  y:    %u\n", map_state_data.view.y);

dump:
	if (fp)
		fclose(fp);

	abort();
}

static void
handle_keydown(const struct event_key *ev)
{
	assert(ev);

	switch (ev->key) {
	case KEY_q:
		game_quit();
		break;
	case KEY_s:
		dump();
		break;
	default:
		break;
	}
}

static void
handle(const union event *ev)
{
	assert(ev);

	switch (ev->type) {
	case EVENT_KEYDOWN:
		handle_keydown(&ev->key);
		break;
	default:
		break;
	}
}

static void
generate(struct label labels[], size_t labelsz)
{
	assert(labels);

	for (size_t i = 0; i < labelsz; ++i) {
		if (labels[i].texture)
			continue;

		labels[i].texture = font_render(data.font, labels[i].text, FOREGROUND);

		if (!labels[i].texture)
			error_fatal();
	}
}

static void
update(unsigned int ticks)
{
	(void)ticks;

	lerror.text = error();

	generate(headers, NELEM(headers));
	generate(&lerror, 1);
	generate(bottom, NELEM(bottom));
}

static void
draw(void)
{
	int y = PADDING;

	painter_set_target(NULL);
	painter_set_color(BACKGROUND);
	painter_clear();

	/* Header. */
	for (size_t i = 0; i < NELEM(headers); ++i) {
		texture_draw(headers[i].texture, PADDING, y);
		y += texture_height(headers[i].texture) + 2;
	}

	/* Error message. */
	texture_draw(lerror.texture, PADDING, y + PADDING);

	/* Bottom. */
	y = window_height() - PADDING;
	y -= texture_height(bottom[0].texture);

	for (size_t i = 0; i < NELEM(bottom); ++i) {
		texture_draw(bottom[i].texture, PADDING, y);
		y -= texture_height(bottom[i].texture) + 2;
	}
}

static void
leave(void)
{
}

struct state panic_state = {
	.enter = enter,
	.handle = handle,
	.update = update,
	.draw = draw,
	.leave = leave
};

void
panic_state_init(void)
{
	/*
	 * If the panic state can not be loaded we're unable to show any
	 * useful information to the screen so as last resort print them
	 * on the console.
	 */
	if (!(data.font = font_openf(sys_datapath(FONT), FONT_SZ)))
		error_fatal();
}

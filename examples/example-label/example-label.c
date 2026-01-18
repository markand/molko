/*
 * example-label.c -- show how to use label and alignments
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

#include <stddef.h>
#include <stdio.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/event.h>
#include <mlk/core/game.h>
#include <mlk/core/painter.h>
#include <mlk/core/panic.h>
#include <mlk/core/sys.h>
#include <mlk/core/util.h>
#include <mlk/core/window.h>

#include <mlk/ui/align.h>
#include <mlk/ui/label.h>
#include <mlk/ui/ui.h>

#include <mlk/example/example.h>
#include <mlk/example/glower.h>

/* Custom delegate/style for glowing one. */
static struct mlk_label_style style_glow;
static struct mlk_glower glow = {
	.start  = 0xffce7fff,
	.end    = 0xd58d6bff,
	.delay  = 22
};

static const char * const theme_names[] = {
	[MLK_WINDOW_THEME_AUTO] = "auto",
	[MLK_WINDOW_THEME_DARK] = "dark",
	[MLK_WINDOW_THEME_LIGHT] = "light"
};

static char theme_help_text[128];

static struct {
	enum mlk_align align;
	struct mlk_label label;
} table[] = {
	{
		.align = MLK_ALIGN_CENTER,
		.label = {
			.text = theme_help_text
		}
	},
	{
		.align = MLK_ALIGN_CENTER,
		.label = {
			.text = "The world is Malikania.",
			.style = &style_glow
		}
	},
	{
		.align = MLK_ALIGN_TOP_LEFT,
		.label = {
			.text = "Top left"
		}
	},
	{
		.align = MLK_ALIGN_TOP,
		.label = {
			.text = "Top"
		}
	},
	{
		.align = MLK_ALIGN_TOP_RIGHT,
		.label = {
			.text = "Top right"
		}
	},
	{
		.align = MLK_ALIGN_RIGHT,
		.label = {
			.text = "Right"
		}
	},
	{
		.align = MLK_ALIGN_BOTTOM_RIGHT,
		.label = {
			.text = "Bottom right"
		}
	},
	{
		.align = MLK_ALIGN_BOTTOM,
		.label = {
			.text = "Bottom"
		}
	},
	{
		.align = MLK_ALIGN_BOTTOM_LEFT,
		.label = {
			.text = "Bottom left"
		}
	},
	{
		.align = MLK_ALIGN_LEFT,
		.label = {
			.text = "Left"
		}
	}
};

static void
style_glow_update(struct mlk_label_style *self, struct mlk_label *label, unsigned int ticks)
{
	(void)label;

	mlk_glower_update(&glow, ticks);
	self->color = glow.color;
}

static void
set_help_text(void)
{
	struct mlk_label *l = &table[0].label;
	unsigned int w, h;

	snprintf(theme_help_text, sizeof (theme_help_text), "Press space to change theme (%s)",
	    theme_names[mlk_window.theme_user]);

	mlk_label_query(l, &w, &h);
	mlk_align(MLK_ALIGN_CENTER, &l->x, &l->y, w, h, 0, 0, mlk_window.w, mlk_window.h);
	l->y += h * 2;
}

static void
init(void)
{
	unsigned int w, h;
	struct mlk_label *l;

	if (mlk_example_init("example-label") < 0)
		mlk_panic();

	/* Change the glowing style. */
	style_glow.update = style_glow_update;

	for (size_t i = 0; i < MLK_UTIL_SIZE(table); ++i) {
		l = &table[i].label;
		mlk_label_query(l, &w, &h);
		mlk_align(table[i].align, &l->x, &l->y, w, h, 0, 0, mlk_window.w, mlk_window.h);
	}

	/* Move a bit the help text. */
	table[0].label.y += 20;

	/* Initialize theme help text. */
	set_help_text();

	mlk_glower_init(&glow);
}

static void
handle(const union mlk_event *ev)
{
	enum mlk_window_theme theme;

	mlk_ui_handle(ev);

	switch (ev->type) {
	case MLK_EVENT_KEYDOWN:
		if (ev->key.key == MLK_KEY_SPACE) {
			theme = (mlk_window.theme_user + 1) % MLK_WINDOW_THEME_LAST;
			mlk_window_set_theme(theme);
			mlk_ui_set_theme(mlk_window.theme_effective);
			set_help_text();
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
update(unsigned int ticks)
{
	for (size_t i = 0; i < MLK_UTIL_SIZE(table); ++i)
		mlk_label_update(&table[i].label, ticks);
}

static void
draw(void)
{
	if (mlk_window.theme_effective == MLK_WINDOW_THEME_DARK)
		mlk_painter_set_color(0x323558ff);
	else
		mlk_painter_set_color(0xdcd4ffff);

	mlk_painter_clear();

	for (size_t i = 0; i < MLK_UTIL_SIZE(table); ++i)
		mlk_label_draw(&table[i].label);

	mlk_painter_present();
}

static void
run(void)
{
	struct mlk_game_ops ops = {
		.handle = handle,
		.update = update,
		.draw = draw
	};

	mlk_game_init(&ops);
	mlk_game_loop();
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

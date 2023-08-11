#include <assert.h>
#include <string.h>

#include "style.h"
#include "ui.h"

/* https://lospec.com/palette-list/duel */

#define NORMAL_COLOR_BG         (0xf5f7faff)
#define NORMAL_COLOR_FG         (0x000000ff)
#define NORMAL_COLOR_BORDER     (0xcdd2daff)
#define NORMAL_COLOR_TEXT       (0x222323ff)
#define NORMAL_GEO_BORDER       (2)
#define NORMAL_GEO_PADDING      (10)
#define NORMAL_FONT             (MLK_UI_FONT_INTERFACE)

#define SELECTED_COLOR_BG       (0x328ca7ff)
#define SELECTED_COLOR_FG       (0x000000ff)
#define SELECTED_COLOR_BORDER   (0x006f89ff)
#define SELECTED_COLOR_TEXT     (0xf5f7faff)
#define SELECTED_GEO_BORDER     (2)
#define SELECTED_GEO_PADDING    (10)
#define SELECTED_FONT           (MLK_UI_FONT_INTERFACE)

struct mlk_style mlk_style = {
	.normal = {
		.color = {
			.bg      = NORMAL_COLOR_BG,
			.fg      = NORMAL_COLOR_FG,
			.border  = NORMAL_COLOR_BORDER,
			.text    = NORMAL_COLOR_TEXT
		},
		.geo = {
			.border  = NORMAL_GEO_BORDER,
			.padding = NORMAL_GEO_PADDING
		},
		.font = &mlk_ui_fonts[NORMAL_FONT]
	},
	.selected = {
		.color = {
			.bg      = SELECTED_COLOR_BG,
			.fg      = SELECTED_COLOR_FG,
			.border  = SELECTED_COLOR_BORDER,
			.text    = SELECTED_COLOR_TEXT
		},
		.geo = {
			.border  = SELECTED_GEO_BORDER,
			.padding = SELECTED_GEO_PADDING
		},
		.font = &mlk_ui_fonts[SELECTED_FONT]
	}
};

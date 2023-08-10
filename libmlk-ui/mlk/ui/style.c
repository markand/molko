#include <assert.h>
#include <string.h>

#include "style.h"
#include "ui.h"

struct mlk_style mlk_style = {
	.padding        = MLK_STYLE_PADDING,
	.bg_color       = MLK_STYLE_BG_COLOR,
	.fg_color       = MLK_STYLE_FG_COLOR,
	.border_color   = MLK_STYLE_BORDER_COLOR,
	.border_size    = MLK_STYLE_BORDER_SIZE,
	.text_color     = MLK_STYLE_TEXT_COLOR,
	.selected_color = MLK_STYLE_SELECTED_COLOR,
	.animation      = MLK_STYLE_ANIMATION,
	.text_font      = &mlk_ui_fonts[MLK_UI_FONT_INTERFACE]
};

void
mlk_style_init(struct mlk_style *style)
{
	assert(style);

	memset(style, 0, sizeof (*style));
}

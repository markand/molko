#ifndef MLK_UI_STYLE_H
#define MLK_UI_STYLE_H

/* https://lospec.com/palette-list/duel */

#define MLK_STYLE_PADDING               (10)
#define MLK_STYLE_BG_COLOR              (0xf5f7faff)
#define MLK_STYLE_FG_COLOR              (0x000000ff)
#define MLK_STYLE_BORDER_COLOR          (0xcdd2daff)
#define MLK_STYLE_BORDER_SIZE           (2)
#define MLK_STYLE_TEXT_COLOR            (0x222323ff)
#define MLK_STYLE_SELECTED_COLOR        (0x55b67dff)
#define MLK_STYLE_ANIMATION             (500)

struct mlk_font;

struct mlk_style {
	unsigned int padding;
	unsigned long bg_color;
	unsigned long fg_color;
	unsigned long border_color;
	unsigned int border_size;
	unsigned long text_color;
	unsigned long selected_color;
	unsigned int animation;
	struct mlk_font *text_font;
};

extern struct mlk_style mlk_style;

void
mlk_style_init(struct mlk_style *style);

#endif /* MLK_UI_STYLE_H */

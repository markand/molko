#ifndef MLK_UI_STYLE_H
#define MLK_UI_STYLE_H

struct mlk_font;

struct mlk_style_color {
	unsigned long bg;
	unsigned long fg;
	unsigned long border;
	unsigned long text;
};

struct mlk_style_geo {
	unsigned short border;
	unsigned short padding;
};

struct mlk_style_attr {
	struct mlk_style_color color;
	struct mlk_style_geo geo;
	struct mlk_font *font;
};

struct mlk_style {
	struct mlk_style_attr normal;
	struct mlk_style_attr selected;
};

extern struct mlk_style mlk_style;

#endif /* MLK_UI_STYLE_H */

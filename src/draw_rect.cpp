#include "draw_rect.h"
#include <graphics.h>

void rect_show(rect rt) {
	color_t fill_color_pre = getfillcolor();
	setfillcolor(rt.color);

	ege_fillrect(rt.x, rt.y, rt.x_size, rt.y_size);

	setfillcolor(fill_color_pre);
}

void rect_appear(rect t, int fps) {
	double a = 0, da = 255 / fps, eps = 1e-6;
	;
}
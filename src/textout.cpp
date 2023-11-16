#include "textout.h"
#include <graphics.h>
#include <stdlib.h>

text text_set[32];

int text_cnt;

void text_init() {
	for (int i = 0; i < 32; i++) {
		strcpy(text_set[i].str, "");
		text_set[i].x = text_set[i].y = 0;
	}
}

void text_cpy(int u, int v) {
	strcpy(text_set[u].str, text_set[v].str);
	text_set[u].x = text_set[v].x;
	text_set[u].y = text_set[v].y;
	text_set[u].color = text_set[v].color;
	text_set[u].font_size = text_set[v].font_size;
	strcpy(text_set[u].font_name, text_set[v].font_name);
}

void text_update(int loc, char str[], int x, int y, color_t color, int font_size, char font_name[]) {
	strcpy(text_set[loc].str, str);
	text_set[loc].x = x;
	text_set[loc].y = y;
	text_set[loc].color = color;
	text_set[loc].font_size = font_size;
	strcpy(text_set[loc].font_name, font_name);
}

void text_show(int loc) {
	color_t color_pre = getcolor();
	LOGFONTA font_pre;
	getfont(&font_pre);

	setfont(text_set[loc].font_size, 0, text_set[loc].font_name);
	setcolor(text_set[loc].color);

	xyprintf(text_set[loc].x, text_set[loc].y, text_set[loc].str);

	setfont(&font_pre);
	setcolor(color_pre);
}

void text_appear(int loc) { //渐入
	int a = 0, da = 8;
	text_set[loc].color %= (1 << 24);
	for (int i = 0; i < 32; i++, delay_fps(60)) { //逐帧循环
		text_show(loc);
		text_set[loc].color += a * (1 << 24);
	}
	text_set[loc].color |= (255ll * (1ll << 24));
}

void text_move(int loc, int dx, int dy, int f, void draw(), bool show_other_text) { //在f帧内移动完成
	double x = text_set[loc].x, y = text_set[loc].y, eps = 1e-6;
	double dx_ = static_cast<double>(dx) / f, dy_ = static_cast<double>(dy) / f;
	for (int i = 0; i < f; i++, delay_fps(60)) { //逐帧循环
		text_set[loc].x = x + eps;
		text_set[loc].y = y + eps;

		draw();
		if (show_other_text) {
			for (int j = 0; j < text_cnt; j++) if (j != loc) {
				text_show(j);
			}
		}
		text_show(loc);
		x += dx_; y += dy_;
	}
}
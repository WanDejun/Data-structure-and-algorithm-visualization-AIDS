#pragma once
#ifndef DREW_ARROW
#define DREW_ARROW
typedef struct ARROW {
	int x_st, y_st, x_ed, y_ed;
	bool visible;
} arrow; //¼ýÍ·¼¯ºÏ

extern arrow arrow_set[32];

void arrow_init();
void arrow_cpy(int u, int v);
void draw_arrow(arrow t);
void draw_arrow(int loc, bool flag = true);
void arrow_appear(int loc, int x_st, int y_st, int x_ed, int y_ed);
void arrow_disappear(int loc, void draw());
#endif // !DREW_ARROW

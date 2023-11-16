#include "draw_arrow.h"
#include <stdio.h>
#include <graphics.h>
#include <math.h>

arrow arrow_set[32];

void arrow_cpy(int u, int v) { //辅助箭头数组
	arrow_set[u].x_st = arrow_set[v].x_st;
	arrow_set[u].x_ed = arrow_set[v].x_ed;
	arrow_set[u].y_st = arrow_set[v].y_st;
	arrow_set[u].y_ed = arrow_set[v].y_ed;
}

void draw_arrow(int loc, bool flag) { // flag == 0表示不在该函数中调整颜色（渐入渐出使用）
	int x_st = arrow_set[loc].x_st,
		x_ed = arrow_set[loc].x_ed,
		y_st = arrow_set[loc].y_st,
		y_ed = arrow_set[loc].y_ed;

	double theta = atan2(y_ed - y_st, x_ed - x_st);

	color_t color_pre = getfillcolor();
	if (flag) setfillcolor(EGEARGB(0xff, 0x00, 0x00, 0x00));

	printf("%f\n", theta);

	ege_point pt[6] = {}; // 箭身部分宽度为4px
	pt[0].x = roundl(x_st - 2 * sin(theta));		//  1---------------4
	pt[0].y = roundl(y_st + 2 * cos(theta));		//  |				|
	pt[1].x = roundl(x_st + 2 * sin(theta));		//  |				|
	pt[1].y = roundl(y_st - 2 * cos(theta));		//  st				ed
	pt[2].x = roundl(x_ed + 2 * sin(theta));		//  |				|
	pt[2].y = roundl(y_ed - 2 * cos(theta));		//	|				|
	pt[3].x = roundl(x_ed - 2 * sin(theta));		//  2---------------3
	pt[3].y = roundl(y_ed + 2 * cos(theta));

	ege_fillpoly(4, pt);

	pt[0].x = x_ed; pt[0].y = y_ed;
	pt[3].x = roundl(x_ed + 5 * cos(theta)); pt[3].y = roundl(y_ed + 5 * sin(theta));
	theta += 3 * PI / 4;
	pt[1].x = roundl(x_ed + 15 * cos(theta)); pt[1].y = roundl(y_ed + 15 * sin(theta));
	theta -= 3 * PI / 4;
	pt[2].x = roundl(pt[1].x + 5 * cos(theta)); pt[2].y = roundl(pt[1].y + 5 * sin(theta));
	theta -= 3 * PI / 4;
	pt[5].x = roundl(x_ed + 15 * cos(theta)); pt[5].y = roundl(y_ed + 15 * sin(theta));
	theta += 3 * PI / 4;
	pt[4].x = roundl(pt[5].x + 5 * cos(theta)); pt[4].y = roundl(pt[5].y + 5 * sin(theta));

	ege_fillpoly(6, pt);

	setfillcolor(color_pre);
}

void draw_arrow(arrow t) { // 绘制箭头，颜色由外部控制（用于淡出）
	int x_st = t.x_st,
		x_ed = t.x_ed,
		y_st = t.y_st,
		y_ed = t.y_ed;

	double theta = atan2(y_ed - y_st, x_ed - x_st);

	color_t color_pre = getfillcolor();

	printf("%f\n", theta);

	ege_point pt[6] = {}; // 箭身部分宽度为4px
	pt[0].x = roundl(x_st - 2 * sin(theta));		//  1---------------4
	pt[0].y = roundl(y_st + 2 * cos(theta));		//  |				|
	pt[1].x = roundl(x_st + 2 * sin(theta));		//  |				|
	pt[1].y = roundl(y_st - 2 * cos(theta));		//  st				ed
	pt[2].x = roundl(x_ed + 2 * sin(theta));		//  |				|
	pt[2].y = roundl(y_ed - 2 * cos(theta));		//	|				|
	pt[3].x = roundl(x_ed - 2 * sin(theta));		//  2---------------3
	pt[3].y = roundl(y_ed + 2 * cos(theta));

	ege_fillpoly(4, pt);

	pt[0].x = x_ed; pt[0].y = y_ed;
	pt[3].x = roundl(x_ed + 5 * cos(theta)); pt[3].y = roundl(y_ed + 5 * sin(theta));
	theta += 3 * PI / 4;
	pt[1].x = roundl(x_ed + 15 * cos(theta)); pt[1].y = roundl(y_ed + 15 * sin(theta));
	theta -= 3 * PI / 4;
	pt[2].x = roundl(pt[1].x + 5 * cos(theta)); pt[2].y = roundl(pt[1].y + 5 * sin(theta));
	theta -= 3 * PI / 4;
	pt[5].x = roundl(x_ed + 15 * cos(theta)); pt[5].y = roundl(y_ed + 15 * sin(theta));
	theta += 3 * PI / 4;
	pt[4].x = roundl(pt[5].x + 5 * cos(theta)); pt[4].y = roundl(pt[5].y + 5 * sin(theta));

	ege_fillpoly(6, pt);

	setfillcolor(color_pre);
}

void arrow_appear(int loc, int x_st, int y_st, int x_ed, int y_ed) { //增加一个箭头到loc位置
	color_t fill_color_pre = getfillcolor();
	int a = 0, da = 8;

	arrow_set[loc].x_st = x_st;
	arrow_set[loc].y_st = y_st;
	arrow_set[loc].x_ed = x_ed;
	arrow_set[loc].y_ed = y_ed;

	for (int i = 0; i < 32; i++, delay_fps(60)) { //逐帧循环
		setfillcolor(EGEARGB(a, 0x00, 0x00, 0x00));
		draw_arrow(loc, 0);
		a += da;
	}

	setfillcolor(fill_color_pre);
}

void arrow_disappear(int loc, void draw()) { //删除loc处的箭头 ，淡出
	arrow t = arrow_set[loc];

	arrow_set[loc].x_st = 0;

	int a = 255, da = 8;
	for (int i = 0; i < 32; i++, delay_fps(60)) { //逐帧循环
		draw();
		setfillcolor(EGEARGB(a, 0x00, 0x00, 0x00));
		draw_arrow(t);
		a -= da;
	}
}
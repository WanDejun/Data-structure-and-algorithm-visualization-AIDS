#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include "draw_arrow.h"

void draw_arrow(int x_st, int y_st, int x_ed, int y_ed, bool flag) {
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
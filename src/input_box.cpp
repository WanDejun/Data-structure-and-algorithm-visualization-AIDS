#include "input_box.h"
#include <graphics.h>
#include <stdlib.h>
#include "draw_rect.h"
#include "textout.h"

//绘制输入器 （50 - 220） * （400 - 690）

const int box_size = 50, space_size = 10; 

typedef struct {
	rect rt; //方格
	text txt; //文本
} rect_with_text; //方格和字符

rect_with_text rect_set[12]; //12个按键 (0-9, BACK, ENTER)
rect screen; //回显背景
rect_with_text quit;
rect_with_text rd;
text input_num[2]; //回显数字 [0]为低位（在右） [1]为高位（在左）

void input_box_init() {
	//rect_set初始化
	for (int i = 0; i < 12; i++) {
		rect_set[i].rt.x = 50 + 60 * (i % 3);
		rect_set[i].rt.y = 460 + 60 * static_cast<int>(i / 3);
		rect_set[i].rt.x_size = 50;
		rect_set[i].rt.y_size = 50;
		rect_set[i].rt.color = EGEARGB(255, 230, 217, 226);
		rect_set[i].txt.color = EGEARGB(255, 0x00, 0x00, 0x00);
		strcpy(rect_set[i].txt.font_name, "Hack");
		rect_set[i].txt.font_size = 20;
		rect_set[i].txt.x = 50 + 60 * (i % 3) + 18;
		rect_set[i].txt.y = 460 + 60 * static_cast<int>(i / 3) + 16;
	}
	strcpy(rect_set[0].txt.str, "7");
	strcpy(rect_set[1].txt.str, "8");
	strcpy(rect_set[2].txt.str, "9");
	strcpy(rect_set[3].txt.str, "4");
	strcpy(rect_set[4].txt.str, "5");
	strcpy(rect_set[5].txt.str, "6");
	strcpy(rect_set[6].txt.str, "1");
	strcpy(rect_set[7].txt.str, "2");
	strcpy(rect_set[8].txt.str, "3");
	strcpy(rect_set[9].txt.str, "0");
	strcpy(rect_set[10].txt.str, "CLR");
	strcpy(rect_set[11].txt.str, "ENTER");
	rect_set[10].txt.x -= 10;
	rect_set[11].txt.x -= 18;
	rect_set[11].txt.font_size = 18;
	rect_set[11].txt.y += 3;
	rect_set[11].rt.color = EGEARGB(255, 0xEF, 0x00, 0x00);

	//screen初始化
	screen.color = EGEARGB(255, 31, 31, 31);
	screen.x = 50;
	screen.y = 400;
	screen.x_size = 170;
	screen.y_size = 50;
	
	//input_num初始化
	for (int i = 0; i < 2; i++) {
		input_num[i].color = EGEARGB(255, 200, 200, 200);
		strcpy(input_num[i].font_name, "Hack");
		input_num[i].font_size = 40;
	}
	strcpy(input_num[0].str, "0");
	strcpy(input_num[1].str, "0");
	input_num[0].x = 190;
	input_num[0].y = 405;
	input_num[1].x = 170;
	input_num[1].y = 405;

	//quit初始化
	strcpy(quit.txt.str, "QUIT");
	quit.txt.color = EGEARGB(255, 0x200, 0x200, 0x200);
	quit.txt.font_size = 22;
	strcpy(quit.txt.font_name, "Hack");
	quit.txt.x = 1180 + 13;
	quit.txt.y = 20 + 18;

	quit.rt.color = EGEARGB(255, 188, 36, 36);
	quit.rt.x = 1180;
	quit.rt.y = 20;
	quit.rt.x_size = 80;
	quit.rt.y_size = 50;

	//rand初始化
	strcpy(rd.txt.str, "RAND");
	rd.txt.color = EGEARGB(255, 0x00, 0x220, 0x155);
	rd.txt.font_size = 18;
	strcpy(rd.txt.font_name, "Hack");
	rd.txt.x = 230 + 5;
	rd.txt.y = 460 + 20;

	rd.rt.color = EGEARGB(255, 188, 36, 36);
	rd.rt.x = 230;
	rd.rt.y = 460;
	rd.rt.x_size = 50;
	rd.rt.y_size = 50;
}

void input_box_show() { //输出整个小键盘
	for (int i = 0; i < 12; i++) {
		rect_show(rect_set[i].rt);
		text_show(rect_set[i].txt);
	}
	rect_show(screen);

	text_show(input_num[0]);
	text_show(input_num[1]);

	rect_show(quit.rt);
	text_show(quit.txt);

	rect_show(rd.rt);
	text_show(rd.txt);
}

void input_box_flush() {
	rect_show(screen);

	text_show(input_num[0]);
	text_show(input_num[1]);
}

void input_box_show_with(void draw()) { //在draw的背景上输出小键盘
	draw();
	input_box_show();
}

void press_num(int num) {
	input_num[1].str[0] = input_num[0].str[0];
	input_num[0].str[0] = '0' + num;
}

void press_clr() {
	input_num[0].str[0] = '0';
	input_num[1].str[0] = '0';
}

void press_rand() {
	input_num[0].str[0] = '0' + rand() % 10;
	input_num[1].str[0] = '0' + rand() % 10;
}

int input_box_get() { //获取输入, 返回输入值，若输入quit返回-1，目前只写了鼠标点击可视化小键盘输入， 有空了可以写通过键盘输入的代码
	input_num[0].str[0] = input_num[1].str[0] = '0';

	int x, y, pre_n = -1;
	mouse_msg msg = { 0 };
	double time_pre = fclock(), time;

	input_box_show();

	for (; is_run(); delay_fps(60)) {
		input_box_flush();

		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		time = fclock();
		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (y > 460 && y < 510) {
			if (x > 50 && x < 100 && (pre_n != 7 || time - time_pre >= 0.2)) {
				pre_n = 7;
				press_num(7);
			}
			else if (x > 110 && x < 160 && (pre_n != 8 || time - time_pre >= 0.2)) {
				pre_n = 8;
				press_num(8);
			}
			else if (x > 170 && x < 220 && (pre_n != 9 || time - time_pre >= 0.2)) {
				pre_n = 9;
				press_num(9);
			}
			else if (x > 230 && x < 280 && (pre_n != 10 || time - time_pre >= 0.2)) {
				pre_n = 10;
				press_rand();
			}
		}
		else if (y > 520 && y < 570) {
			if (x > 50 && x < 100 && (pre_n != 4 || time - time_pre >= 0.2)) {
				pre_n = 4;
				press_num(4);
			}
			else if (x > 110 && x < 160 && (pre_n != 5 || time - time_pre >= 0.2)) {
				pre_n = 5;
				press_num(5);
			}
			else if (x > 170 && x < 220 && (pre_n != 6 || time - time_pre >= 0.2)) {
				pre_n = 6;
				press_num(6);
			}
		}
		else if (y > 580 && y < 630) {
			if (x > 50 && x < 100 && (pre_n != 1 || time - time_pre >= 0.2)) {
				pre_n = 1;
				press_num(1);
			}
			else if (x > 110 && x < 160 && (pre_n != 2 || time - time_pre >= 0.2)) {
				pre_n = 2;
				press_num(2);
			}
			else if (x > 170 && x < 220 && (pre_n != 3 || time - time_pre >= 0.2)) {
				pre_n = 3;
				press_num(3);
			}
		}
		else if (y > 640 && y < 690 && (pre_n != 0 || time - time_pre >= 0.2)) {
			if (x > 50 && x < 100) {
				pre_n = 0;
				press_num(0);
			}
			else if (x > 110 && x < 160 && (pre_n != 10 || time - time_pre >= 0.2)) {
				pre_n = 10;
				press_clr();
			}
			else if (x > 170 && x < 220 && (pre_n != 11 || time - time_pre >= 0.2)) {
				pre_n = 11;
				return (input_num[1].str[0] - '0') * 10 + input_num[0].str[0] - '0';
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				return -1;
			}
		}
		time_pre = time;
	}
	return 0;
}
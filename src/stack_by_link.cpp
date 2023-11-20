#include "stack_by_link.h"
#include <graphics.h>
#include <stdio.h>
#include "textout.h"
#include "draw_rect.h"
#include "draw_arrow.h"
#include "input_box.h"
#include "draw_error.h"
#include "typedef.h"

const int stack_by_link_max_size = 7, stack_by_link_bk_px = 50, text_det_x = 17, text_det_y = 16;

rect_with_text stack_by_link_rect_set[stack_by_link_max_size + 1]; //+1是为了存NULL节点
arrow stack_by_link_arrow_set[stack_by_link_max_size];

arrow_with_text top_text;

int stack_by_link_top = 0;

bool stack_by_link_quit_flag;

void stack_by_link_init() { 
	stack_by_link_quit_flag = 0;

	for (int x = 350, y = 335, i = 0; i < stack_by_link_max_size + 1; i++, x += stack_by_link_bk_px * 2) {
		//初始化rect
		stack_by_link_rect_set[i].rt.x = x;
		stack_by_link_rect_set[i].rt.y = y;
		stack_by_link_rect_set[i].rt.x_size = stack_by_link_bk_px;
		stack_by_link_rect_set[i].rt.y_size = stack_by_link_bk_px;
		stack_by_link_rect_set[i].rt.color = EGEARGB(255, 86, 156, 186);

		strcpy(stack_by_link_rect_set[i].txt.font_name, "Hack");
		stack_by_link_rect_set[i].txt.font_size = 20;
		stack_by_link_rect_set[i].txt.x = x + text_det_x;
		stack_by_link_rect_set[i].txt.y = y + text_det_y; //两位数字要在创建节点时后移，以保证居中
		stack_by_link_rect_set[i].txt.color = BLACK;

		stack_by_link_rect_set[i].visible = 0;
	}

	stack_by_link_rect_set[0].visible = 1;
	stack_by_link_rect_set[0].rt.color = EGEARGB(255, 51, 51, 51);
	strcpy(stack_by_link_rect_set[0].txt.str, "NULL");
	stack_by_link_rect_set[0].txt.x -= 14;
	stack_by_link_rect_set[0].txt.color = EGEARGB(255, 200, 200, 200);

	for (int i = 0; i < stack_by_link_max_size; i++) { //初始化arrow_set
		stack_by_link_arrow_set[i].x_st = stack_by_link_rect_set[i + 1].rt.x - 5;
		stack_by_link_arrow_set[i].x_ed = stack_by_link_rect_set[i].rt.x + 10 + stack_by_link_bk_px;
		stack_by_link_arrow_set[i].y_st = stack_by_link_rect_set[i + 1].rt.y + stack_by_link_bk_px / 2;
		stack_by_link_arrow_set[i].y_ed = stack_by_link_rect_set[i].rt.y + stack_by_link_bk_px / 2;

		stack_by_link_arrow_set[i].visible = 0;
	}

	top_text.aw.x_ed = stack_by_link_rect_set[0].rt.x + stack_by_link_bk_px / 2;
	top_text.aw.y_ed = stack_by_link_rect_set[0].rt.y - 20;
	top_text.aw.x_st = top_text.aw.x_ed;
	top_text.aw.y_st = top_text.aw.y_ed - 30;

	top_text.txt.color = BLACK;
	strcpy(top_text.txt.font_name, "Hack");
	top_text.txt.font_size = 30;
	strcpy(top_text.txt.str, "top");
	top_text.txt.x = top_text.aw.x_st - 25;
	top_text.txt.y = top_text.aw.y_st - 35;

	top_text.visible = 1;
}

void stack_by_link_draw() {
	cleardevice();

	for (int i = 0; i <= stack_by_link_max_size; i++) { //绘制方格和文字（节点元素）
		if (stack_by_link_rect_set[i].visible) {
			rect_show(stack_by_link_rect_set[i].rt);
			text_show(stack_by_link_rect_set[i].txt);
		}
	}
	for (int i = 0; i < stack_by_link_max_size; i++) { //绘制箭头
		if (stack_by_link_arrow_set[i].visible) {
			setfillcolor(BLACK);
			draw_arrow(stack_by_link_arrow_set[i]);
		}
	}
	setfillcolor(BLACK);
	if (top_text.visible) {
		draw_arrow(top_text.aw);
		text_show(top_text.txt);
	}
}

void stack_by_link_top_move_back() {
	double x = 0, dx = static_cast<double>(100) / 30 + 1e-6;
	int detx;
	top_text.visible = 0;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		stack_by_link_draw();
		detx = static_cast<int>(x + dx) - static_cast<int>(x);
		x += dx;

		top_text.aw.x_ed += detx;
		top_text.aw.x_st += detx;

		top_text.txt.x += detx;

		text_show(top_text.txt);
		draw_arrow(top_text.aw);
	}

	top_text.visible = 1;
}

void stack_by_link_appear(int val) {
	sprintf(stack_by_link_rect_set[stack_by_link_top].txt.str, "%d", val);
	if (val / 10) stack_by_link_rect_set[stack_by_link_top].txt.x -= 5;

	int a = -1, da = 4;

	for (int i = 0; i < 64; i++, delay_fps(60)) {
		a += da;

		stack_by_link_draw();

		stack_by_link_rect_set[stack_by_link_top].rt.color %= (1ll << 24);
		stack_by_link_rect_set[stack_by_link_top].txt.color %= (1ll << 24);

		stack_by_link_rect_set[stack_by_link_top].rt.color |= (a << 24);
		stack_by_link_rect_set[stack_by_link_top].txt.color |= (a << 24);

		rect_show(stack_by_link_rect_set[stack_by_link_top].rt);
		text_show(stack_by_link_rect_set[stack_by_link_top].txt);

		setfillcolor(EGEARGB(a, 0x00, 0x00, 0x00));
		draw_arrow(stack_by_link_arrow_set[stack_by_link_top - 1]);
	}

	stack_by_link_rect_set[stack_by_link_top].visible = 1;
	stack_by_link_arrow_set[stack_by_link_top - 1].visible = 1;
}

void stack_by_link_push() {
	stack_by_link_draw();

	int val = input_box_get(); //获取输入

	if (val == -1) { //判断quit
		stack_by_link_quit_flag = 1;
		return;
	}

	if (stack_by_link_top > stack_by_link_max_size) {
		draw_error();
		return;
	}

	stack_by_link_top_move_back(); //指针后移
	stack_by_link_top++;

	stack_by_link_appear(val); //新增节点
}

void stack_by_link_top_move_front() {
	double x = 0, dx = static_cast<double>(100) / 30 + 1e-6;
	int detx;
	top_text.visible = 0;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		stack_by_link_draw();
		detx = static_cast<int>(x + dx) - static_cast<int>(x);
		x += dx;

		top_text.aw.x_ed -= detx;
		top_text.aw.x_st -= detx;

		top_text.txt.x -= detx;

		text_show(top_text.txt);
		draw_arrow(top_text.aw);
	}

	top_text.visible = 1;
}

void stack_by_link_disappear() {
	int a = 256, da = 4;

	stack_by_link_rect_set[stack_by_link_top].visible = 0;
	stack_by_link_arrow_set[stack_by_link_top - 1].visible = 0;

	for (int i = 0; i < 64; i++, delay_fps(60)) {
		a -= da;

		stack_by_link_draw();

		stack_by_link_rect_set[stack_by_link_top].rt.color %= (1ll << 24);
		stack_by_link_rect_set[stack_by_link_top].txt.color %= (1ll << 24);

		stack_by_link_rect_set[stack_by_link_top].rt.color |= (a << 24);
		stack_by_link_rect_set[stack_by_link_top].txt.color |= (a << 24);

		rect_show(stack_by_link_rect_set[stack_by_link_top].rt);
		text_show(stack_by_link_rect_set[stack_by_link_top].txt);

		setfillcolor(EGEARGB(a, 0x00, 0x00, 0x00));
		draw_arrow(stack_by_link_arrow_set[stack_by_link_top - 1]);
	}

	if (stack_by_link_rect_set[stack_by_link_top].txt.str[1] != '\0') stack_by_link_rect_set[stack_by_link_top].txt.x += 5;
}

void stack_by_link_pop() {
	stack_by_link_draw();

	if (stack_by_link_top == 0) {
		draw_error();
		return;
	}

	stack_by_link_disappear(); //删除节点

	stack_by_link_top_move_front(); //指针前移
	stack_by_link_top--;
}

void stack_by_link_UI() {
	rect_with_text key[3];
	rect_with_text quit;

	//初始化两个按键和输出提示
	for (int i = 0; i < 3; i++) {
		key[i].rt.x = 300;
		key[i].rt.y = 400 + i * 100;
		key[i].rt.x_size = 150;
		key[i].rt.y_size = 80;

		key[i].txt.color = EGEARGB(0xff, 0x00, 0x00, 0x00);
		strcpy(key[i].txt.font_name, "Hack");
		key[i].txt.font_size = 35;
		key[i].txt.x = key[i].rt.x + 32;
		key[i].txt.y = key[i].rt.y + 24;
	}
	key[2].txt.x += 10;

	key[0].rt.color = EGEARGB(0, 0, 0, 0); //透明背景
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //绿
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //蓝

	key[0].txt.x = 300;

	strcpy(key[0].txt.str, "NEXT STEP:");
	strcpy(key[1].txt.str, "PUSH");
	strcpy(key[2].txt.str, "POP");


	//初始化quit
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

	//输出
	for (int i = 0; i < 3; i++) {
		rect_show(key[i].rt);
		text_show(key[i].txt);
	}
	rect_show(quit.rt);
	text_show(quit.txt);
}

void stack_by_link_main() {
	stack_by_link_init();
	stack_by_link_draw();

	int x, y;
	mouse_msg msg = { 0 };

	for (; is_run() && !stack_by_link_quit_flag; delay_fps(60)) {
		stack_by_link_UI();

		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (x > 300 && x < 450) {
			if (y > 500 && y < 580) {
				stack_by_link_push();
			}
			else if (y > 600 && y < 680) {
				stack_by_link_pop();
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				stack_by_link_quit_flag = 1;
			}
		}
	}
}
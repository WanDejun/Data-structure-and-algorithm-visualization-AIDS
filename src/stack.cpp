#include "stack.h"
#include <graphics.h>
#include "draw_rect.h"
#include "input_box.h"
#include "textout.h"
#include <stdio.h>
#include "grey.h"
#include "random_color.h"
#include "draw_error.h"

rect stack_back_ground[3];
text bottom;

typedef struct {
	rect rt; //方格
	text txt; //文本
	bool visible;
} rect_with_text; //方格和字符

const int stack_max_size = 7, bk_x_size = 200, bk_y_size = 60, text_det_y = 20;

rect_with_text stack_rect_set[stack_max_size];

text top_txt;
bool txt_flag;


int top, stack[stack_max_size];

bool stack_quit_flag;

void stack_init() { // (700 - 900) * (240 - 660) 最大共7（stack_max_size）个block
	text_init();
	
	stack_quit_flag = 0; //退出标识

	top = 0;

	top_txt.color = BLACK;
	strcpy(top_txt.font_name, "Hack");
	top_txt.font_size = 30;
	strcpy(top_txt.str, "top ->");
	top_txt.x = 580;
	top_txt.y = 620;

	txt_flag = 1;

	stack_back_ground[0].color = BLACK; //左
	stack_back_ground[0].x = 698;
	stack_back_ground[0].y = 210;
	stack_back_ground[0].x_size = 2;
	stack_back_ground[0].y_size = 450;

	stack_back_ground[1].color = BLACK; //右
	stack_back_ground[1].x = 900;
	stack_back_ground[1].y = 210;
	stack_back_ground[1].x_size = 2;
	stack_back_ground[1].y_size = 450;

	stack_back_ground[2].color = BLACK; // 下底
	stack_back_ground[2].x = 698;
	stack_back_ground[2].y = 660;
	stack_back_ground[2].x_size = 204;
	stack_back_ground[2].y_size = 8;

	bottom.color = BLACK;
	strcpy(bottom.font_name, "Hack");
	bottom.font_size = 30;
	strcpy(bottom.str, "Bottom");
	bottom.x = 750;
	bottom.y = 670;

	for (int i = 0; i < stack_max_size; i++) { //初始化stack_rect_set, 颜色在push时添加， txt.x需要根据数字长度修改
		stack_rect_set[i].visible = 0;

		stack_rect_set[i].rt.x = 700;
		stack_rect_set[i].rt.x_size = 200;
		stack_rect_set[i].rt.y = 600 - i * bk_y_size;
		stack_rect_set[i].rt.y_size = bk_y_size;

		strcpy(stack_rect_set[i].txt.font_name,"Hack");
		stack_rect_set[i].txt.font_size = 30;
		stack_rect_set[i].txt.x = 790;
		stack_rect_set[i].txt.y = 600 - i * bk_y_size + text_det_y; // 差20
	}
}

void stack_draw_bk() {
	for (int i = 0; i < 3; i++) {
		rect_show(stack_back_ground[i]);
	}
	text_show(bottom);
}

void stack_draw() {
	cleardevice();

	for (int i = 0; i < stack_max_size; i++) {
		if (stack_rect_set[i].visible) {
			rect_show(stack_rect_set[i].rt);
			text_show(stack_rect_set[i].txt);
		}
	}

	stack_draw_bk();
	if (txt_flag) text_show(top_txt);
}

void stack_new_block(int loc, int val) { //产生一个新的节点
	stack_rect_set[loc].rt.color = rand_color();

	int gray = get_grey(stack_rect_set[loc].rt.color); //获取灰度

	if (gray < 128) stack_rect_set[loc].txt.color = EGEARGB(0xcc, 0xff, 0xff, 0xff); //灰度偏黑，字体设置白色
	else stack_rect_set[loc].txt.color = EGEARGB(0xff, 0x00, 0x00, 0x00); //灰度偏白，字体设置黑色

	stack_rect_set[loc].txt.x -= 10 * (static_cast<int>(val / 10) ? 1 : 0);

	sprintf(stack_rect_set[loc].txt.str, "%d", val);
}

void stack_move_down(int loc) { //
	double y = 10, a = 0, eps = 1e-6;
	double dy = (stack_rect_set[loc].rt.y - y) / 60, da = static_cast<double>(255) / 30;


	stack_rect_set[loc].rt.y = y;
	stack_rect_set[loc].txt.y = y + text_det_y;

	for (int i = 0; i < 30; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		a += da;

		stack_rect_set[loc].rt.color %= (1 << 24);
		stack_rect_set[loc].rt.color |= static_cast<int>(a + eps) << 24;
		rect_show(stack_rect_set[loc].rt);

		stack_rect_set[loc].txt.color %= (1 << 24);
		stack_rect_set[loc].txt.color |= static_cast<int>(a + eps) << 24;
		text_show(stack_rect_set[loc].txt);
	}

	for (int i = 0; i < 60; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		y += dy;

		stack_rect_set[loc].rt.y = y + eps;
		rect_show(stack_rect_set[loc].rt);

		stack_rect_set[loc].txt.y = y + text_det_y + eps;
		text_show(stack_rect_set[loc].txt);
	}

	stack_rect_set[loc].visible = 1;
}

void stack_top_move(bool flag) { //0表示up, 1表示down
	txt_flag = 0;

	double y = top_txt.y, dy = static_cast<double>(bk_y_size) / 50, eps = 1e-6;

	for (int i = 0; i < 50; i++, delay_fps(60)) {
		stack_draw();

		if (flag) y += dy;
		else y -= dy;

		top_txt.y = y;
		text_show(top_txt);
	}

	txt_flag = 1;
}

void stack_push() {
	stack_draw();

	if (top == stack_max_size) {
		draw_error();
		return;
	}

	int val = input_box_get();

	if (val == -1) {
		stack_quit_flag = 1;
		return;
	}

	stack_new_block(top, val);
	stack_move_down(top);
	stack_top_move(0);

	stack[top] = val;
	top++;

	stack_draw();
}

void stack_move_up(int loc) { //方格向上移动
	double y = stack_rect_set[loc].rt.y, a = 255, eps = 1e-6;
	double dy = (y - 10) / 60, da = static_cast<double>(255) / 30;

	rect_with_text t = stack_rect_set[loc];

	stack_rect_set[loc].visible = 0;
	stack_rect_set[loc].txt.x += 10 * (static_cast<int>(stack_rect_set[loc].txt.str[1] == '\0') ? 0 : 1);

	t.rt.y = y;
	t.txt.y = y + text_det_y;

	for (int i = 0; i < 60; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		y -= dy;

		t.rt.y = y - eps;
		rect_show(t.rt);

		t.txt.y = y + text_det_y - eps;
		text_show(t.txt);
	}

	for (int i = 0; i < 30; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		a -= da;

		t.rt.color %= (1 << 24);
		t.rt.color |= static_cast<int>(a + eps) << 24;
		rect_show(t.rt);

		t.txt.color %= (1 << 24);
		t.txt.color |= static_cast<int>(a + eps) << 24;
		text_show(t.txt);
	}
}

void stack_pop() {
	stack_draw();
	if (top == 0) {
		draw_error();
		return;
	}

	top--;

	stack_move_up(top);
	stack_top_move(1);

	stack_draw();
}

void stack_UI() { // 不清屏， 覆盖打印 x:(300-450)px y:(400-480, 500-580, 600-680)px
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

void stack_main() {
	stack_init();
	stack_draw();

	int x, y;
	mouse_msg msg = { 0 };

	for (; is_run() && !stack_quit_flag; delay_fps(60)) {
		stack_UI();

		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (x > 300 && x < 450) {
			if (y > 500 && y < 580) {
				stack_push();
			}
			else if (y > 600 && y < 680) {
				stack_pop();
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				stack_quit_flag = 1;
			}
		}
	}
}
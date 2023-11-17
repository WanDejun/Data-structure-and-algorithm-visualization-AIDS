#include "stack.h"
#include <graphics.h>
#include "draw_rect.h"
#include "input_box.h"
#include "textout.h"
#include "random_color.h"
#include "grey.h"

rect stack_back_ground[3];
text bottom;

typedef struct {
	rect rt; //方格
	text txt; //文本
	bool visible;
} rect_with_text; //方格和字符

const int stack_max_size = 7, bk_x_size = 200, bk_y_size = 80;

rect_with_text stack_rect_set[stack_max_size];

text top_txt;
int top, stack[stack_max_size];

bool stack_quit_flag;

void stack_init() { // (700 - 900) * (240 - 660) 最大共7（stack_max_size）个block
	text_init();
	
	top = 0;

	top_txt.color = BLACK;
	strcpy(top_txt.font_name, "Hack");
	top_txt.font_size = 30;
	strcpy(top_txt.str, "top ->");
	top_txt.x = 580;
	top_txt.y = 620;

	stack_quit_flag = 0;

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
		stack_rect_set[i].rt.y = 580 - i * 80;
		stack_rect_set[i].rt.y_size = 80;

		strcpy(stack_rect_set[i].txt.font_name,"Hack");
		stack_rect_set[i].txt.font_size = 30;
		stack_rect_set[i].txt.x = 790;
		stack_rect_set[i].txt.y = 600 - i * 80; // 差20
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

	stack_draw_bk();
	for (int i = 0; i < stack_max_size; i++) {
		if (stack_rect_set[i].visible) {
			text_show(stack_rect_set[i].txt);
			rect_show(stack_rect_set[i].rt);
		}
	}
	text_show(top_txt);
}

void stack_new_block(int loc) {
	stack_rect_set[loc].rt.color = rand_color();

	int gray = get_grey(stack_rect_set[loc].rt.color);

	if (gray < 128) stack_rect_set[loc].txt.color = BLACK;
	else stack_rect_set[loc].txt.color = WHITE;

	stack_rect_set[loc].txt.x += 30;


}

void stack_move_down(int loc) { //
	double y = 10, a = 0, eps = 1e-6;
	double dy = (stack_rect_set[loc].rt.y - y) / 60, da = 255 / 30;

	for (int i = 0; i < 30; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		a += da;

		stack_rect_set[loc].rt.color %= (1 << 24);
		stack_rect_set[loc].rt.color += static_cast<int>(a + eps) << 24;
		rect_show(stack_rect_set[loc].rt);

		stack_rect_set[loc].txt.color %= (1 << 24);
		stack_rect_set[loc].txt.color += static_cast<int>(a + eps) << 24;
		text_show(stack_rect_set[loc].txt);
	}

	for (int i = 0; i < 60; i++, delay_fps(60)) { //逐帧循环
		stack_draw();

		y += dy;

		stack_rect_set[loc].rt.y = y + eps;
		rect_show(stack_rect_set[loc].rt);

		stack_rect_set[loc].txt.y = y + 20 + eps;
		text_show(stack_rect_set[loc].txt);
	}

	stack_rect_set[loc].visible = 1;
}

void stack_push() {
	int val = input_box_get();

	if (val == -1) {
		stack_quit_flag = 1;
		return;
	}

	stack_new_block(top);
	stack_move_down(top);

	stack[top] = val;
	top++;
	stack_draw();
	Sleep(10000);
}
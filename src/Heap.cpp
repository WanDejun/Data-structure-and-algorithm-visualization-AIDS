//大根堆
#include "heap.h"
#include <graphics.h>
#include <stdio.h>
#include "draw_arrow.h"
#include "typedef.h"
#include "draw_error.h"
#include "textout.h"
#include "input_box.h"

static const int heap_max_size = 7, heap_det_x = 6, heap_det_y = 12, heap_x_space = 200, heap_y_space = 150;

static cycle_with_text heap_node_set[heap_max_size + 2];

static arrow heap_arrow_set[heap_max_size + 2];

static cycle heap_red_cycle;

static int heap_size;

static text heap_info;

static bool heap_quit_flag;

static void heap_init() {
	heap_quit_flag = 0; //退出标识置0

	heap_size = 0; //节点大小

	//初始化节点
	for (int i = 1; i <= heap_max_size + 1; i++) {
		heap_node_set[i].cyc.color = EGERGB(90, 240, 210);
		heap_node_set[i].cyc.r = 25;

		heap_node_set[i].txt.color = BLACK;
		strcpy(heap_node_set[i].txt.font_name, FONT);
		heap_node_set[i].txt.font_size = 20;

		heap_node_set[i].visible = 0;
	}
	heap_node_set[1].cyc.x = 750; //初始化头结点
	heap_node_set[1].cyc.y = 200;
	heap_node_set[1].txt.x = heap_node_set[1].cyc.x - heap_det_x;
	heap_node_set[1].txt.y = heap_node_set[1].cyc.y - heap_det_y;

	//初始化arrow_set
	for (int i = 1; i <= heap_max_size + 1; i++) {
		heap_arrow_set[i].visible = 0;
	}

	//初始化heap_info
	heap_info.color = BLACK;
	strcpy(heap_info.font_name, FONT);
	heap_info.font_size = 30;
	heap_info.x = 80;
	heap_info.y = 80;
	strcpy(heap_info.str, "");
}

static void heap_draw() {
	cleardevice();

	for (int i = 1; i <= heap_max_size; i++) {
		if (heap_node_set[i].visible) {
			cycle_show(heap_node_set[i].cyc);
			text_show(heap_node_set[i].txt);
		}
	}

	setfillcolor(BLACK);
	for (int i = 1; i <= heap_max_size; i++) {
		if (heap_arrow_set[i].visible) {
			draw_arrow(heap_arrow_set[i]);
		}
	}

	text_show(heap_info);
}

static void stretch() { //因为堆只有三层，此处做的是简易版，只用伸展第二层
	//strcpy(heap_info.str, "");

	double cyc_x1 = heap_node_set[2].cyc.x, cyc_x2 = heap_node_set[3].cyc.x, txt_x1 = heap_node_set[2].txt.x, txt_x2 = heap_node_set[3].txt.x;
	double dx = static_cast<double>(heap_x_space) / 2 / 60, eps = 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		cyc_x1 -= (dx - eps);
		cyc_x2 += (dx + eps);
		txt_x1 -= (dx - eps);
		txt_x2 += (dx + eps);

		heap_node_set[2].cyc.x = cyc_x1;
		heap_node_set[3].cyc.x = cyc_x2;
		heap_node_set[2].txt.x = txt_x1;
		heap_node_set[3].txt.x = txt_x2;

		heap_arrow_set[2].x_ed = cyc_x1 + 20;
		heap_arrow_set[3].x_ed = cyc_x2 - 20;

		heap_draw();
	}
}

static void red_cycle_flash(int loc) {
	cycle_cpy(&heap_red_cycle, heap_node_set[loc].cyc);
	heap_red_cycle.color = RED;

	cycle_show(heap_red_cycle);
	Sleep(200);

	heap_red_cycle.color = heap_node_set[loc].cyc.color;
	cycle_show(heap_red_cycle);
}

static void adjust(int loc) {
	int max_loc = loc << 1;
	if ((loc << 1 | 1) <= heap_size && s2i(heap_node_set[loc << 1 | 1].txt.str) > s2i(heap_node_set[loc << 1].txt.str)) 
		max_loc = loc << 1 | 1; //选择较大的子节点

	if (s2i(heap_node_set[max_loc].txt.str) <= s2i(heap_node_set[loc].txt.str)) return;

	red_cycle_flash(loc);

	double x1 = heap_node_set[loc].txt.x, x2 = heap_node_set[max_loc].txt.x, dx = (x2 - x1) / 100, eps = 1e-6;
	double y1 = heap_node_set[loc].txt.y, y2 = heap_node_set[max_loc].txt.y, dy = (y2 - y1) / 100;

	for (int i = 0; i < 100; i++, delay_fps(60)) {
		x1 += (dx + eps);
		x2 -= (dx - eps);
		y1 += (dy + eps);
		y2 -= (dy - eps);

		heap_node_set[loc].txt.x = x1;
		heap_node_set[max_loc].txt.x = x2;
		heap_node_set[loc].txt.y = y1;
		heap_node_set[max_loc].txt.y = y2;

		heap_draw();
	}

	//交换字符
	char str[3];
	strcpy(str, heap_node_set[loc].txt.str);
	strcpy(heap_node_set[loc].txt.str, heap_node_set[max_loc].txt.str);
	strcpy(heap_node_set[max_loc].txt.str, str);

	//交换x坐标(通过cyc的坐标恢复)
	heap_node_set[loc].txt.x = heap_node_set[loc].cyc.x - heap_det_x;
	heap_node_set[max_loc].txt.x = heap_node_set[max_loc].cyc.x - heap_det_x;
	if (heap_node_set[loc].txt.str[1] != '\0') heap_node_set[loc].txt.x -= 5;
	if (heap_node_set[max_loc].txt.str[1] != '\0') heap_node_set[max_loc].txt.x -= 5;


	//交换y坐标
	heap_node_set[loc].txt.y = heap_node_set[loc].cyc.y - heap_det_y;
	heap_node_set[max_loc].txt.y = heap_node_set[max_loc].cyc.y - heap_det_y;

	heap_draw();

	if ((max_loc << 1) <= heap_size) { Sleep(200); adjust(max_loc); }
}

static void heap_appear(int val) {
	sprintf(heap_info.str, "heap[%d] = val;", heap_size);

	if (heap_size != 1) {
		if (heap_size & 1) { //右节点
			heap_node_set[heap_size].cyc.x = heap_node_set[heap_size >> 1].cyc.x + heap_x_space / 2; //新节点圆心

			heap_arrow_set[heap_size].x_st = heap_node_set[heap_size >> 1].cyc.x + 20; //箭头初始与终止的x坐标
			heap_arrow_set[heap_size].x_ed = heap_node_set[heap_size].cyc.x - 25;
		}
		else { //左节点
			heap_node_set[heap_size].cyc.x = heap_node_set[heap_size >> 1].cyc.x - heap_x_space / 2; //新节点圆心

			heap_arrow_set[heap_size].x_st = heap_node_set[heap_size >> 1].cyc.x - 20;
			heap_arrow_set[heap_size].x_ed = heap_node_set[heap_size].cyc.x + 25;
		}
		//初始化新节点的其他变量
		heap_node_set[heap_size].cyc.y = heap_node_set[heap_size >> 1].cyc.y + heap_y_space;
		heap_node_set[heap_size].txt.x = heap_node_set[heap_size].cyc.x - heap_det_x;
		heap_node_set[heap_size].txt.y = heap_node_set[heap_size].cyc.y - heap_det_y;

		//初始化新指针的其他变量
		heap_arrow_set[heap_size].y_st = heap_node_set[heap_size >> 1].cyc.y + 20;
		heap_arrow_set[heap_size].y_ed = heap_node_set[heap_size].cyc.y - 25;
		heap_arrow_set[heap_size].visible = 1;
	}

	heap_node_set[heap_size].visible = 1;

	sprintf(heap_node_set[heap_size].txt.str, "%d", val);
	if (val >= 10) heap_node_set[heap_size].txt.x -= 5;

	double a = 0, da = static_cast<double>(255) / 30 + 1e-6;
	int i_a;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		a += da;
		i_a = a;

		heap_node_set[heap_size].cyc.color %= (1 << 24);
		heap_node_set[heap_size].cyc.color |= (i_a << 24);

		heap_node_set[heap_size].txt.color %= (1 << 24);
		heap_node_set[heap_size].txt.color |= (i_a << 24);

		setfillcolor(EGEACOLOR(i_a, BLACK));
		draw_arrow(heap_arrow_set[heap_size]);

		heap_draw();
	}
}

static void heap_push() {
	if (heap_size == heap_max_size) {
		draw_error();
		return;
	}

	int val = input_box_get();

	if (val == -1) {
		heap_quit_flag = 1;
		return;
	}

	heap_size++;

	if (heap_size == 4) stretch();

	heap_appear(val);

	for (int i = heap_size; i > 1; i >>= 1) {
		sprintf(heap_info.str, "adjust(%d);", i >> 1);
		adjust(i >> 1);
		Sleep(200);
	}
}

static void heap_swap_disappear() {
	int f = 50;
	double x = heap_node_set[1].txt.x, y = heap_node_set[1].txt.y, dx = 0, dy = static_cast<double>(100) / f, eps = 1e-6;
	double a = 255, da = static_cast<double>(255) / f;
	for (int i = 0; i < f; i++, delay_fps(60)) { //根节点值渐出
		y -= dy;
		a -= da;

		heap_node_set[1].txt.y = y;
		heap_node_set[1].txt.color %= (1 << 24);
		heap_node_set[1].txt.color |= (static_cast<int>(a) << 24);

		heap_draw();
	}

	//heap_size节点的值赋给根节点

	text_cpy(&heap_node_set[1].txt, heap_node_set[heap_size].txt);
}

static void heap_pop() {
	if (heap_size == 0) {
		draw_error();
		return;
	}

	heap_swap_disappear();

	heap_size--;
}

void heap_main() {
	heap_init();
	heap_draw();

	heap_push();
	heap_pop();

}
#include "heap.h"
#include <graphics.h>
#include <stdio.h>
#include "draw_arrow.h"
#include "typedef.h"
#include "draw_error.h"
#include "textout.h"
#include "input_box.h"

static const int heap_max_size = 7, heap_det_x = 7, heap_det_y = 12, heap_x_space = 200, heap_y_space = 150;

static cycle_with_text heap_node_set[heap_max_size + 2];

static arrow heap_arrow_set[heap_max_size + 2];

static cycle head_red_cycle;

static int heap_size;

static text heap_info;

static bool heap_quit_flag;

void heap_init() {
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

	cycle_cpy(head_red_cycle, heap_node_set[1].cyc);
	head_red_cycle.color = RED;
}

void heap_draw() {
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

static void stretch() {

}

static void adjust(int loc) {
	int max_loc = loc << 1;
	if ((loc << 1 | 1) <= heap_size && s2i(heap_node_set[loc << 1 | 1].txt.str))
}

static void heap_appear(int val) {
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

void heap_insert() {
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
	heap_appear(val);

}

void heap_main() {
	heap_init();
	heap_draw();

	heap_insert();
	heap_insert();
	heap_insert();
}
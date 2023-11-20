#include "queue_by_link.h"
#include <graphics.h>
#include <stdio.h>
#include "draw_arrow.h"
#include "draw_rect.h"
#include "draw_error.h"
#include "input_box.h"
#include "textout.h"

typedef struct {
	rect rt; //方格
	text txt; //文本
	bool visible;
} rect_with_text; //方格和字符

typedef struct {
	arrow aw; // 箭头
	text txt; //文本
	bool visible;
} arrow_with_text; //箭头和字符

bool queue_by_link_quit_flag;

const int queue_by_link_max_size = 7, text_det_x = 17, text_det_y = 16, queue_by_link_bk_size = 50; //间距50px

rect_with_text queue_by_link_node_set[queue_by_link_max_size + 1]; //包含bottom节点

arrow_with_text head_p, tail_p;

arrow queue_by_link_arrow_set[queue_by_link_max_size];

static int head_loc, tail_loc;

void queue_by_link_init() {
	queue_by_link_quit_flag = 0;

	head_loc = tail_loc = 0;

	for (int i = 0, x = 350, y = 335; i <= queue_by_link_max_size; i++, x += 2 * queue_by_link_bk_size) { //x起始坐标350
		queue_by_link_node_set[i].rt.color = EGEARGB(0xff, 0x66, 0xcc, 0xff);
		queue_by_link_node_set[i].rt.x = x;
		queue_by_link_node_set[i].rt.x_size = queue_by_link_bk_size;
		queue_by_link_node_set[i].rt.y = y;
		queue_by_link_node_set[i].rt.y_size = queue_by_link_bk_size;

		queue_by_link_node_set[i].txt.color = BLACK;
		queue_by_link_node_set[i].txt.font_size = 20;
		strcpy(queue_by_link_node_set[i].txt.font_name, "Hack");
		queue_by_link_node_set[i].txt.x = x + text_det_x;
		queue_by_link_node_set[i].txt.y = y + text_det_y;

		queue_by_link_node_set[i].visible = 0;
	}
	queue_by_link_node_set[0].rt.color = EGERGB(51, 51, 51);

	queue_by_link_node_set[0].txt.color = EGERGB(200, 200, 200);
	strcpy(queue_by_link_node_set[0].txt.str, "NULL");
	queue_by_link_node_set[0].txt.x -= 14;

	queue_by_link_node_set[0].visible = 1;

	for (int i = 0; i < queue_by_link_max_size; i++) { //初始化连接箭头
		queue_by_link_arrow_set[i].visible = 0;

		queue_by_link_arrow_set[i].x_st = queue_by_link_node_set[i + 1].rt.x - 5;
		queue_by_link_arrow_set[i].x_ed = queue_by_link_node_set[i].rt.x + queue_by_link_bk_size + 10;
		queue_by_link_arrow_set[i].y_st = queue_by_link_node_set[i + 1].rt.y + queue_by_link_bk_size / 2;
		queue_by_link_arrow_set[i].y_ed = queue_by_link_node_set[i].rt.y + queue_by_link_bk_size / 2;
	}
	
	head_p.aw.x_st = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	head_p.aw.x_ed = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	head_p.aw.y_st = queue_by_link_node_set[0].rt.y - 50;
	head_p.aw.y_ed = queue_by_link_node_set[0].rt.y - 20;
	head_p.visible = 1;

	head_p.txt.color = BLACK;
	strcpy(head_p.txt.font_name, "Hack");
	head_p.txt.font_size = 30;
	strcpy(head_p.txt.str, "Head");
	head_p.txt.x = head_p.aw.x_st - 30;
	head_p.txt.y = head_p.aw.y_st - 35;

	tail_p.aw.x_st = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	tail_p.aw.x_ed = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	tail_p.aw.y_st = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 50;
	tail_p.aw.y_ed = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 20;
	tail_p.visible = 1;

	tail_p.txt.color = BLACK;
	strcpy(tail_p.txt.font_name, "Hack");
	tail_p.txt.font_size = 30;
	strcpy(tail_p.txt.str, "Tail");
	tail_p.txt.x = tail_p.aw.x_st - 30;
	tail_p.txt.y = tail_p.aw.y_st + 35;
}

void queue_by_link_draw() {
	cleardevice();

	for (int i = 0; i <= queue_by_link_max_size; i++) {
		if (queue_by_link_node_set[i].visible) {
			rect_show(queue_by_link_node_set[i].rt);
			text_show(queue_by_link_node_set[i].txt);
		}
	}

	setfillcolor(BLACK);

	for (int i = 0; i < queue_by_link_max_size; i++) {
		if (queue_by_link_arrow_set[i].visible) {
			draw_arrow(queue_by_link_arrow_set[i]);
		}
	}

	if (head_p.visible) {
		text_show(head_p.txt);
		draw_arrow(head_p.aw);
	}
	if (tail_p.visible) {
		text_show(tail_p.txt);
		draw_arrow(tail_p.aw);
	}
}

void queue_by_link_node_appear(int val) {
	double a = 0, da = static_cast<double>(255) / 60 + 1e-6;
	int i_a = 0;

	sprintf(queue_by_link_node_set[tail_loc + 1].txt.str, "%d", val);
	if (val >= 10) queue_by_link_node_set[tail_loc + 1].txt.x -= 5;

	queue_by_link_node_set[tail_loc + 1].visible = 1;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		a += da;
		i_a = a;

		queue_by_link_node_set[tail_loc + 1].rt.color %= (1 << 24);
		queue_by_link_node_set[tail_loc + 1].rt.color |= (i_a << 24);

		queue_by_link_node_set[tail_loc + 1].txt.color %= (1 << 24);
		queue_by_link_node_set[tail_loc + 1].txt.color |= (i_a << 24);

		queue_by_link_draw();
	}

	a = 0;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		a += da;
		i_a = a;

		setfillcolor(EGEARGB(i_a, 0xff, 0xff, 0xff));

		queue_by_link_draw();
		draw_arrow(queue_by_link_arrow_set[tail_loc]);
	}
	queue_by_link_arrow_set[tail_loc].visible = 1;
}

void queue_by_link_tail_back() {
	double aw_x = tail_p.aw.x_st, txt_x = tail_p.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		aw_x += dx;
		txt_x += dx;

		tail_p.aw.x_st = tail_p.aw.x_ed = aw_x;
		tail_p.txt.x = txt_x;
		queue_by_link_draw();
	}
}

void queue_by_link_push() {
	queue_by_link_draw();

	if (tail_loc == queue_by_link_max_size) {
		draw_error();
		return;
	}

	int val = input_box_get();

	if (val == -1) {
		queue_by_link_quit_flag = 1;
		return;
	}

	queue_by_link_node_appear(val);
	queue_by_link_tail_back();
}

void queue_by_link_main() {
	queue_by_link_init();
	queue_by_link_draw();

	queue_by_link_push();
}
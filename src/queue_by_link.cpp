#include "queue_by_link.h"
#include <graphics.h>
#include <stdio.h>
#include "draw_arrow.h"
#include "draw_rect.h"
#include "draw_error.h"
#include "input_box.h"
#include "textout.h"
#include "typedef.h"

bool queue_by_link_quit_flag;

const int queue_by_link_max_size = 7, text_det_x = 17, text_det_y = 16, queue_by_link_bk_size = 50; //间距50px

rect_with_text queue_by_link_node_set[queue_by_link_max_size + 1]; 

rect_with_text null_node; //null节点

arrow_with_text queue_by_link_head_p, queue_by_link_tail_p;

arrow queue_by_link_arrow_set[queue_by_link_max_size];

static int queue_by_link_head_loc, queue_by_link_tail_loc;

void queue_by_link_init() {
	queue_by_link_quit_flag = 0;

	queue_by_link_head_loc = queue_by_link_tail_loc = 0;

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

	//初始化null_node
	null_node.rt.color = EGERGB(51, 51, 51);
	null_node.rt.x = queue_by_link_node_set[0].rt.x;
	null_node.rt.x_size = queue_by_link_bk_size;
	null_node.rt.y = queue_by_link_node_set[0].rt.y;
	null_node.rt.y_size = queue_by_link_bk_size;

	null_node.txt.color = EGERGB(200, 200, 200);
	strcpy(null_node.txt.str, "NULL");
	null_node.txt.font_size = 20;
	strcpy(null_node.txt.font_name, "Hack");
	null_node.txt.x = queue_by_link_node_set[0].rt.x + text_det_x - 14;
	null_node.txt.y = queue_by_link_node_set[0].rt.y + text_det_y;

	null_node.visible = 1;

	for (int i = 0; i < queue_by_link_max_size; i++) { //初始化连接箭头
		queue_by_link_arrow_set[i].visible = 0;

		queue_by_link_arrow_set[i].x_st = queue_by_link_node_set[i].rt.x + queue_by_link_bk_size + 5;
		queue_by_link_arrow_set[i].x_ed = queue_by_link_node_set[i + 1].rt.x - 10;
		queue_by_link_arrow_set[i].y_st = queue_by_link_node_set[i].rt.y + queue_by_link_bk_size / 2;
		queue_by_link_arrow_set[i].y_ed = queue_by_link_node_set[i + 1].rt.y + queue_by_link_bk_size / 2;
	}
	
	//初始化head指针
	queue_by_link_head_p.aw.x_st = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_head_p.aw.x_ed = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_head_p.aw.y_st = queue_by_link_node_set[0].rt.y - 50;
	queue_by_link_head_p.aw.y_ed = queue_by_link_node_set[0].rt.y - 20;
	queue_by_link_head_p.visible = 1;

	queue_by_link_head_p.txt.color = BLACK;
	strcpy(queue_by_link_head_p.txt.font_name, "Hack");
	queue_by_link_head_p.txt.font_size = 30;
	strcpy(queue_by_link_head_p.txt.str, "Head");
	queue_by_link_head_p.txt.x = queue_by_link_head_p.aw.x_st - 30;
	queue_by_link_head_p.txt.y = queue_by_link_head_p.aw.y_st - 35;

	//初始化tail指针
	queue_by_link_tail_p.aw.x_st = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_tail_p.aw.x_ed = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_tail_p.aw.y_st = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 50;
	queue_by_link_tail_p.aw.y_ed = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 20;
	queue_by_link_tail_p.visible = 1;

	queue_by_link_tail_p.txt.color = BLACK;
	strcpy(queue_by_link_tail_p.txt.font_name, "Hack");
	queue_by_link_tail_p.txt.font_size = 30;
	strcpy(queue_by_link_tail_p.txt.str, "Tail");
	queue_by_link_tail_p.txt.x = queue_by_link_tail_p.aw.x_st - 30;
	queue_by_link_tail_p.txt.y = queue_by_link_tail_p.aw.y_st + 35;
}

void queue_by_link_draw() {
	cleardevice();

	rect_show(null_node.rt);
	text_show(null_node.txt);

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

	if (queue_by_link_head_p.visible) {
		text_show(queue_by_link_head_p.txt);
		draw_arrow(queue_by_link_head_p.aw);
	}
	if (queue_by_link_tail_p.visible) {
		text_show(queue_by_link_tail_p.txt);
		draw_arrow(queue_by_link_tail_p.aw);
	}
}

void queue_by_link_null_back() {
	double rt_x = null_node.rt.x, txt_x = null_node.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		rt_x += dx;
		txt_x += dx;

		null_node.rt.x = rt_x;
		null_node.txt.x = txt_x;

		queue_by_link_draw();
	}
}

void queue_by_link_node_appear(int val) {
	double a = 0, da = static_cast<double>(255) / 30 + 1e-6;
	int i_a = 0;

	sprintf(queue_by_link_node_set[queue_by_link_tail_loc].txt.str, "%d", val);
	if (val >= 10) queue_by_link_node_set[queue_by_link_tail_loc].txt.x -= 5;

	queue_by_link_node_set[queue_by_link_tail_loc].visible = 1;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		a += da;
		i_a = a;

		queue_by_link_node_set[queue_by_link_tail_loc].rt.color %= (1 << 24);
		queue_by_link_node_set[queue_by_link_tail_loc].rt.color |= (i_a << 24);

		queue_by_link_node_set[queue_by_link_tail_loc].txt.color %= (1 << 24);
		queue_by_link_node_set[queue_by_link_tail_loc].txt.color |= (i_a << 24);

		queue_by_link_draw();
	}

	a = 0;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		a += da;
		i_a = a;

		queue_by_link_draw();

		setfillcolor(EGEARGB(i_a, 0x00, 0x00, 0x00));
		draw_arrow(queue_by_link_arrow_set[queue_by_link_tail_loc]);
	}

	queue_by_link_arrow_set[queue_by_link_tail_loc].visible = 1;
}

void queue_by_link_tail_back() {
	double aw_x = queue_by_link_tail_p.aw.x_st, txt_x = queue_by_link_tail_p.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		aw_x += dx;
		txt_x += dx;

		queue_by_link_tail_p.aw.x_st = queue_by_link_tail_p.aw.x_ed = aw_x;
		queue_by_link_tail_p.txt.x = txt_x;
		queue_by_link_draw();
	}
}

void queue_by_link_head_back() {
	double aw_x = queue_by_link_head_p.aw.x_st, txt_x = queue_by_link_head_p.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		aw_x += dx;
		txt_x += dx;

		queue_by_link_head_p.aw.x_st = queue_by_link_head_p.aw.x_ed = aw_x;
		queue_by_link_head_p.txt.x = txt_x;
		queue_by_link_draw();
	}
}

void queue_by_link_push() {
	queue_by_link_draw();

	if (queue_by_link_tail_loc == queue_by_link_max_size) {
		draw_error();
		return;
	}

	int val = input_box_get();

	if (val == -1) {
		queue_by_link_quit_flag = 1;
		return;
	}

	queue_by_link_null_back();
	queue_by_link_node_appear(val);
	if (queue_by_link_tail_loc != 0) queue_by_link_tail_back();
	queue_by_link_tail_loc++;
}

void queue_by_link_node_disappear() {
	double a = 255, da = static_cast<double>(255) / 30 + 1e-6;
	int i_a = 0;

	queue_by_link_arrow_set[queue_by_link_head_loc].visible = 0;

	for (int i = 0; i < 30; i++, delay_fps(60)) {
		a -= da;
		i_a = a;

		queue_by_link_node_set[queue_by_link_head_loc].rt.color %= (1 << 24);
		queue_by_link_node_set[queue_by_link_head_loc].rt.color |= (i_a << 24);

		queue_by_link_node_set[queue_by_link_head_loc].txt.color %= (1 << 24);
		queue_by_link_node_set[queue_by_link_head_loc].txt.color |= (i_a << 24);

		queue_by_link_draw();

		setfillcolor(EGEARGB(i_a, 0x00, 0x00, 0x00));
		draw_arrow(queue_by_link_arrow_set[queue_by_link_head_loc]);
	}

	queue_by_link_node_set[queue_by_link_head_loc].visible = 0;
}

void queue_by_link_move_front() {
	int dx = 1;

	for (int i = 0; i < 100; i++, delay_fps(60)) {
		for (int i = 0; i < queue_by_link_tail_loc - 1; i++) {
			queue_by_link_arrow_set[i].x_ed -= dx;
			queue_by_link_arrow_set[i].x_st -= dx;

			queue_by_link_node_set[i].rt.x -= dx;
			queue_by_link_node_set[i].txt.x -= dx;
		}


		null_node.rt.x -= dx;
		null_node.txt.x -= dx; 
		
		queue_by_link_head_p.aw.x_st -= dx;
		queue_by_link_head_p.aw.x_ed -= dx;
		queue_by_link_head_p.txt.x -= dx;

		queue_by_link_tail_p.aw.x_st -= dx;
		queue_by_link_tail_p.aw.x_ed -= dx;
		queue_by_link_tail_p.txt.x -= dx;

		queue_by_link_draw();
	}
}

void queue_by_link_pop() {
	queue_by_link_draw();

	if (queue_by_link_tail_loc <= 0) {
		draw_error();
		return;
	}

	queue_by_link_node_disappear();
	if (queue_by_link_tail_loc != 1) queue_by_link_head_back();

	for (int i = 0; i < queue_by_link_tail_loc - 1; i++) { // 节点前移
		queue_by_link_node_set[i].rt.x = queue_by_link_node_set[i + 1].rt.x;
		queue_by_link_node_set[i].rt.color = queue_by_link_node_set[i + 1].rt.color;

		text_cpy(&queue_by_link_node_set[i].txt, &queue_by_link_node_set[i + 1].txt);
	}

	queue_by_link_node_set[queue_by_link_tail_loc - 1].visible = 0; //最后一个节点重置
	if (queue_by_link_node_set[queue_by_link_tail_loc - 1].txt.str[1] != '\0') queue_by_link_node_set[queue_by_link_tail_loc - 1].txt.x -= 5;

	for (int i = 0; i < queue_by_link_tail_loc - 1; i++) { //箭头前移
		queue_by_link_arrow_set[i].x_st += 100;
		queue_by_link_arrow_set[i].x_ed += 100;
		queue_by_link_arrow_set[i].visible = queue_by_link_arrow_set[i + 1].visible;
	}

	queue_by_link_arrow_set[queue_by_link_tail_loc - 1].visible = 0;

	queue_by_link_move_front();

	queue_by_link_tail_loc--;
}

void queue_by_link_main() {
	queue_by_link_init();
	queue_by_link_draw();

	queue_by_link_push();
	queue_by_link_push();
	queue_by_link_push();
	queue_by_link_pop();
	queue_by_link_pop();
}
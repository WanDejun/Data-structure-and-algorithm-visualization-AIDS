
#include "tree_demo.h"
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include "typedef.h"

static const int max_size = 15, det_x = 5, det_y = 8, x_space = 100, y_space = 80, x_mid = 640;

static cycle_with_text node_set[max_size + 2];

static arrow tree_arrow_set[max_size + 2];

static cycle marked_cycle;

static int size;

static double dx_arr[max_size + 2], x_arr[max_size + 2];

static text info;

static bool tree_quit_flag;

static void init() {
	tree_quit_flag = 0; //�˳���ʶ��0

	size = 0; //�ڵ��С

	//��ʼ���ڵ�
	for (int i = 1; i <= max_size + 1; i++) {
		node_set[i].cyc.color = EGERGB(0x39, 0xcc, 0xbb);
		node_set[i].cyc.r = 20;

		node_set[i].txt.color = BLACK;
		strcpy(node_set[i].txt.font_name, FONT);
		node_set[i].txt.font_size = 15;

		node_set[i].visible = 0;
	}
	node_set[1].cyc.x = x_mid; //��ʼ��ͷ���
	node_set[1].cyc.y = 200;
	node_set[1].txt.x = node_set[1].cyc.x - det_x;
	node_set[1].txt.y = node_set[1].cyc.y - det_y;

	//��ʼ��arrow_set
	for (int i = 1; i <= max_size + 1; i++) {
		tree_arrow_set[i].visible = 0;
	}

	//��ʼ��heap_info
	info.color = BLACK;
	strcpy(info.font_name, FONT);
	info.font_size = 30;
	info.x = 80;
	info.y = 50;
	strcpy(info.str, "");
}

static void draw() {
	cleardevice();

	for (int i = 1; i <= max_size; i++) {
		if (node_set[i].visible) {
			text_show(node_set[i].txt);
			cycle_show(node_set[i].cyc);
		}
	}

	setfillcolor(BLACK);
	for (int i = 2; i <= max_size; i++) {
		if (tree_arrow_set[i].visible) {
			draw_arrow(tree_arrow_set[i]);
		}
	}
}

static void appear() {
	if (size != 1) {
		if (size & 1) {
			node_set[size].cyc.x = node_set[size >> 1].cyc.x + x_space / 2;
			tree_arrow_set[size].x_st = node_set[size >> 1].cyc.x + 15;
			tree_arrow_set[size].x_ed = node_set[size].cyc.x - 20;
		}
		else {
			node_set[size].cyc.x = node_set[size >> 1].cyc.x - x_space / 2;
			tree_arrow_set[size].x_st = node_set[size >> 1].cyc.x - 15;
			tree_arrow_set[size].x_ed = node_set[size].cyc.x + 20;
		}
		node_set[size].cyc.y = node_set[size >> 1].cyc.y + y_space;
	}

	tree_arrow_set[size].y_st = node_set[size >> 1].cyc.y + 15;
	tree_arrow_set[size].y_ed = node_set[size].cyc.y - 20;

	node_set[size].visible = 1;

	sprintf(node_set[size].txt.str, "%d", rand() % 100);

	node_set[size].txt.y = node_set[size].cyc.y - det_y;
	node_set[size].txt.x = node_set[size].cyc.x - det_x;
	if (node_set[size].txt.str[1] != '\0') node_set[size].txt.x -= 4;

	int f = 30;
	double a = 0, da = 255.0 / f;

	for (int i = 0; i < f; i++, delay_fps(60)) {
		a += da;

		node_set[size].cyc.color %= (1 << 24);
		node_set[size].txt.color %= (1 << 24);

		node_set[size].cyc.color |= (static_cast<int>(a) << 24);
		node_set[size].txt.color |= (static_cast<int>(a) << 24);

		draw();

		if (size != 1) {
			setfillcolor(EGEACOLOR(int(a), BLACK));
			draw_arrow(tree_arrow_set[size]);
		}
	}
	tree_arrow_set[size].visible = 1;
}

static void push_down(int loc) {
	if ((loc << 1) >= size) return;
	dx_arr[loc << 1] = dx_arr[loc];
	dx_arr[loc << 1 | 1] = dx_arr[loc];
	push_down(loc << 1);
	push_down(loc << 1 | 1);
}

static void stretch() {
	/*
	���ʱ�ڵ���Ϊn�� ����log(n) + 1��
	��log(n) + 1�㹲��n���ڵ�
	��i�㵽�ײ�ľ���Ϊlog(n) + 1 - i
	��ÿ����i��ڵ��Ӧ2 ^ (log(n) + 1 - i)���ڵ㣬����i��ڵ���Ϊx_space * 2 ^ (log(n) + 1 - i)
	ԭ�����Ϊx_space * 2 ^ (log(n) - i)������෭��
	У׼ʱ����i��ڵ��루i - 1�����ƫ����ҲΪ x_space * 2 ^ (log(n) - i)
	*/
	for (int i = 0; i <= max_size; i++) {
		dx_arr[i] = 0;
		x_arr[i] = node_set[i].cyc.x;
	}

	int f = 100;
	int m = log(size) / log(2) + 1e-6 + 1;

	for (int i = 1; i < m - 1; i++) {
		for (int loc = 1 << i; loc < (1 << (i + 1)); loc++) { // ��i + 1��
			if (loc & 1) dx_arr[loc] += (double(x_space) * double(pow(2, m - (i + 1) - 1)) / 2) / f;
			else dx_arr[loc] -= (double(x_space) * double(pow(2, m - (i + 1) - 1)) / 2) / f;
			push_down(loc);
		}
	}

	for (int i = 0; i < f; i++, delay_fps(60)) {
		for (int j = 2; j < size; j++) {
			x_arr[j] += dx_arr[j];

			node_set[j].cyc.x = x_arr[j];

			if (j & 1) { //�ҽڵ�
				tree_arrow_set[j].x_st = x_arr[j >> 1] + 15;
				tree_arrow_set[j].x_ed = x_arr[j] - 20;
			}
			else { //��ڵ�
				tree_arrow_set[j].x_st = x_arr[j >> 1] - 15;
				tree_arrow_set[j].x_ed = x_arr[j] + 20;
			}

			node_set[j].txt.y = node_set[j].cyc.y - det_y;
			node_set[j].txt.x = node_set[j].cyc.x - det_x;
			if (node_set[j].txt.str[1] != '\0') node_set[j].txt.x -= 4;
		}

		draw();
	}

	for (int i = 1; i < m - 1; i++) {
		for (int loc = 1 << i; loc < (1 << (i + 1)); loc++) { // ��i + 1��
			if (loc & 1) {
				node_set[loc].cyc.x = node_set[loc >> 1].cyc.x + x_space * pow(2, (m - 1 - (i + 1)));
				tree_arrow_set[loc].x_st = node_set[loc >> 1].cyc.x + 15;
				tree_arrow_set[loc].x_ed = node_set[loc].cyc.x - 20;
			}
			else {
				node_set[loc].cyc.x = node_set[loc >> 1].cyc.x - x_space * pow(2, (m - 1 - (i + 1)));
				tree_arrow_set[loc].x_st = node_set[loc >> 1].cyc.x - 15;
				tree_arrow_set[loc].x_ed = node_set[loc].cyc.x + 20;
			}

			node_set[loc].txt.y = node_set[loc].cyc.y - det_y;
			node_set[loc].txt.x = node_set[loc].cyc.x - det_x;
			if (node_set[loc].txt.str[1] != '\0') node_set[loc].txt.x -= 4;
		}
	}
	draw();
}

static void insert() {
	size++;

	if ((size & (-size)) == size && size >= 4) { //�ж��Ƿ�Ϊ2�ı���
		stretch();
	}

	appear();
}

void tree_main() {
	init();
	draw();

	for (int i = 0; i < max_size; i++) {
		insert();
	}
}
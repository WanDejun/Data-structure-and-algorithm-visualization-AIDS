#include "link.h"
#include <graphics.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "draw_arrow.h"
/*
7个有效节点，2个哨兵节点，共9个
x方向
每个节点50px 节点间隔50px
双侧间距215px 
y方向
居中(335px-385px)
*/
typedef struct LINK_NODE {
	int val;
	LINK_NODE* next, * pre;
} link_node;

typedef struct LINK_RECT {
	int x, y;
	char str[6];
} link_rect; //方框（节点）集合

const int LINK_MAX_SIZE = 7, LINK_PX = 50, LINK_MAXI = 50; //最大节点数， 节点（方格）的像素， 节点值的最大值

link_node *head, *tail;

int link_x[LINK_MAX_SIZE + 2], link_y[LINK_MAX_SIZE + 2]; //输出定位
int link_node_size = 0; //当前节点数量

link_rect link_arr_rect[LINK_MAX_SIZE + 2]; //两个表用于动画时的更改和输出

void link_rect_cpy(int u, int v) { // 复制方格数组
	strcpy(link_arr_rect[u].str, link_arr_rect[v].str);
	link_arr_rect[u].x = link_arr_rect[v].x;
	link_arr_rect[u].y = link_arr_rect[v].y;
}

void link_init() {
	cleardevice(); //清屏

	for (int x = 215, y = 335, i = 0; i < LINK_MAX_SIZE + 2; i++, x += 100) { link_x[i] = x; link_y[i] = y; } //初始化坐标序列

	head = (link_node*)malloc(sizeof(link_node)); //初始化链表
	tail = (link_node*)malloc(sizeof(link_node));
	if (!head) exit(0);
	if (!tail) exit(0);

	head->next = tail; head->pre = nullptr;
	tail->pre = head; tail->next = nullptr;
	link_node_size = 2;

	strcpy(link_arr_rect[0].str, "Head"); //初始化link_arr_rect表
	link_arr_rect[0].x = link_x[0];
	link_arr_rect[0].y = link_y[0];

	strcpy(link_arr_rect[1].str, "Tail");
	link_arr_rect[1].x = link_x[1];
	link_arr_rect[1].y = link_y[1];

	arrow_set[1].x_st = link_x[0] + LINK_PX + 5; //初始化arrow_set表 奇数为nxet 偶数为pre
	arrow_set[1].y_st = link_y[0] + LINK_PX / 2 - 10;
	arrow_set[1].x_ed = link_x[1] - 10;
	arrow_set[1].y_ed = link_y[1] + LINK_PX / 2 - 10;

	arrow_set[2].x_st = link_x[1] - 10;
	arrow_set[2].y_st = link_y[1] + LINK_PX / 2 + 10;
	arrow_set[2].x_ed = link_x[0] + LINK_PX + 5;
	arrow_set[2].y_ed = link_y[0] + LINK_PX / 2 + 10;
}

void link_draw() { //按照link_arr_rect和arrow_set绘制图形
	link_node* p = head->next;
	int i;

	cleardevice();
	setfillcolor(EGEARGB(255, 0x66, 0xcc, 0xff));
	setcolor(EGEARGB(0xff, 0x00, 0x00, 0x00));
	setfont(20, 0, "Hack");

	for (i = 0; i < link_node_size; i++) { //输出方格
		if (link_arr_rect[i].x == 0) continue; //不输出未初始化的方格
		ege_fillrect(link_arr_rect[i].x, link_arr_rect[i].y, LINK_PX, LINK_PX);
		xyprintf(link_arr_rect[i].x + 5 * (5 - strlen(link_arr_rect[i].str)), link_arr_rect[i].y + 15, "%s", link_arr_rect[i].str);
	}

	for (i = 1; i <= (link_node_size - 1) << 1; i++) { //输出箭头
		if (arrow_set[i].x_st == 0) continue; //不输出未初始化的箭头
		draw_arrow(i);
	}
}

void link_move_back(int loc) {
	double dx = 2; //每帧移动的距离
	for (int i = 0; i < 50; i++, delay_fps(60)) { //逐帧循环
		for (int j = loc; j < link_node_size; j++) {
			link_arr_rect[j].x += dx;
		}

		arrow_set[(loc - 1) << 1 | 1].x_ed += dx;
		arrow_set[loc << 1].x_st += dx;

		for (int j = loc << 1 | 1; j <= (link_node_size - 1) << 1; j++) {
			arrow_set[j].x_st += dx;
			arrow_set[j].x_ed += dx;
		}

		link_draw();
	}
}

void link_insert_rect(int loc, int val) { //插入方格
	for (int i = link_node_size; i > loc; i--) {
		link_rect_cpy(i, i - 1);
	}
	link_arr_rect[loc].x = 0;
	for (int i = link_node_size << 1; i >= (loc + 1) << 1; i--) {
		arrow_cpy(i, i - 2);
	}
	arrow_set[loc << 1].x_st = 0;
	arrow_set[loc << 1 | 1].x_st = 0;
	link_node_size++; //增加节点数

	color_t fill_color_pre = getfillcolor(), color_pre = getcolor();

	int da = 8, a = 0;

	sprintf(link_arr_rect[loc].str, "%d", val);

	for (int i = 0; i < 32; i++, delay_fps(60)) { //逐帧循环,方格渐入
		link_draw();

		setfillcolor(EGEARGB(a, 0x66, 0xcc, 0xff));
		setcolor(EGEARGB(a, 0x00, 0x00, 0x00));

		ege_fillrect(link_x[loc], link_y[loc] - 100, LINK_PX, LINK_PX);
		xyprintf(link_x[loc] + 5 * (5 - strlen(link_arr_rect[loc].str)), link_y[loc] - 100 + 15, "%s", link_arr_rect[loc].str);

		a += da;
	}

	link_arr_rect[loc].x = link_x[loc]; //添加方格实例
	link_arr_rect[loc].y = link_y[loc] - 100;

	setfillcolor(fill_color_pre);
	setcolor(color_pre);
}

void link_insert_arrow(int loc) {
	arrow_appear(loc << 1 | 1, // loc.next = nxt
				 link_x[loc] + LINK_PX + 5, link_y[loc] - 100 + LINK_PX / 2 - 10, 
				 link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 - 10);
	arrow_appear(loc << 1,  //loc.pre = pre
				 link_x[loc] - 10, link_y[loc] - 100 + LINK_PX / 2 + 10, 
				 link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 + 10);

	arrow_disappear((loc - 1) << 1 | 1, link_draw); 
	arrow_appear((loc - 1) << 1 | 1, 
				  link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 - 10, 
				  link_x[loc] - 10, link_y[loc] + LINK_PX / 2 - 10 - 100);
	
	arrow_disappear((loc + 1) << 1, link_draw);
	arrow_appear((loc + 1) << 1, 
				  link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 + 10, 
				  link_x[loc] + LINK_PX + 5, link_y[loc] + LINK_PX / 2 + 10 - 100);
}

void link_move_down(int loc) {
	int dy = 2;
	for (int i = 0; i < 50; i++, delay_fps(60)) { //逐帧循环
		arrow_set[(loc - 1) << 1 | 1].y_ed += dy;
		arrow_set[loc << 1].y_st += dy;
		arrow_set[loc << 1 | 1].y_st += dy;
		arrow_set[(loc + 1) << 1].y_ed += dy;
		link_arr_rect[loc].y += dy;
		link_draw();
	}
}

void link_insert() {
	link_node* p = (link_node*)malloc(sizeof(link_node)), *pre = head, *nxt = head->next;
	if (!p) exit(0);
	p->val = rand() % LINK_MAXI;
	int loc = 1;
	while (nxt != tail) {
		if (nxt->val >= p->val) break;
		pre = nxt;
		nxt = nxt->next;
		loc++;
	}

	link_move_back(loc);

	p->pre = pre;
	p->next = nxt;
	pre->next = p;
	nxt->pre = p;

	link_insert_rect(loc, p->val);
	link_insert_arrow(loc);
	link_move_down(loc);
}

void link_main() {
	link_init();
	link_draw();
	Sleep(1000);

	while (link_node_size < LINK_MAX_SIZE + 2) {
		link_insert();
		link_draw();
		Sleep(500);
	}
}
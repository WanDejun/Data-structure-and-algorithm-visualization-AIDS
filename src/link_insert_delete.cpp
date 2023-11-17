#include "link_insert_delete.h"
#include <graphics.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "draw_arrow.h"
#include "textout.h"
#include "input_box.h"
/*
7����Ч�ڵ㣬2���ڱ��ڵ㣬��9��
x����
ÿ���ڵ�50px �ڵ���50px
˫����215px 
y����
����(335px-385px)
*/
typedef struct LINK_NODE {
	int val;
	LINK_NODE* next, * pre;
} link_node;

typedef struct LINK_RECT {
	int x, y;
	char str[6];
} link_rect; //���򣨽ڵ㣩����

const int LINK_MAX_SIZE = 7, LINK_PX = 50, LINK_MAXI = 50; //���ڵ����� �ڵ㣨���񣩵����أ� �ڵ�ֵ�����ֵ

link_node *head, *tail;

int link_x[LINK_MAX_SIZE + 2], link_y[LINK_MAX_SIZE + 2]; //�����λ
int link_node_size = 0; //��ǰ�ڵ�����

link_rect link_arr_rect[LINK_MAX_SIZE + 2]; //���������ڶ���ʱ�ĸ��ĺ����

bool quit_flag; //����Ƿ�������quit��

void link_rect_cpy(int u, int v) { // ���Ʒ�������
	strcpy(link_arr_rect[u].str, link_arr_rect[v].str);
	link_arr_rect[u].x = link_arr_rect[v].x;
	link_arr_rect[u].y = link_arr_rect[v].y;
}

void link_init() {
	cleardevice(); //����

	arrow_init();

	quit_flag = 0;

	for (int x = 215, y = 335, i = 0; i < LINK_MAX_SIZE + 2; i++, x += 100) { link_x[i] = x; link_y[i] = y; } //��ʼ����������

	text_update(2, "", 100, 100, EGEARGB(255, 0x00, 0x00, 0x00), 30, "Hack");

	text_init();

	//test_set [0]���ڴ�pre�� [1]���ڴ�nxt
	text_update(0, "pre", link_x[0] + 15, link_y[0] + LINK_PX + 15, EGEARGB(255, 0x00, 0x00, 0x00), 20, "Hack"); 
	text_update(1, "nxt", link_x[1] + 15, link_y[1] + LINK_PX + 30, EGEARGB(255, 0x00, 0x00, 0x00), 20, "Hack");

	text_update(2, "", 100, 100, EGEARGB(255, 0x00, 0x00, 0x00), 30, "Hack");
	text_cnt = 3;

	head = (link_node*)malloc(sizeof(link_node)); //��ʼ������
	tail = (link_node*)malloc(sizeof(link_node));
	if (!head) exit(0);
	if (!tail) exit(0);

	head->next = tail; head->pre = nullptr;
	tail->pre = head; tail->next = nullptr;
	link_node_size = 2;

	strcpy(link_arr_rect[0].str, "Head"); //��ʼ��link_arr_rect��
	link_arr_rect[0].x = link_x[0];
	link_arr_rect[0].y = link_y[0];

	strcpy(link_arr_rect[1].str, "Tail");
	link_arr_rect[1].x = link_x[1];
	link_arr_rect[1].y = link_y[1];

	arrow_set[1].x_st = link_x[0] + LINK_PX + 5; //��ʼ��arrow_set�� ����Ϊnxet ż��Ϊpre
	arrow_set[1].y_st = link_y[0] + LINK_PX / 2 - 10;
	arrow_set[1].x_ed = link_x[1] - 10;
	arrow_set[1].y_ed = link_y[1] + LINK_PX / 2 - 10;

	arrow_set[2].x_st = link_x[1] - 10;
	arrow_set[2].y_st = link_y[1] + LINK_PX / 2 + 10;
	arrow_set[2].x_ed = link_x[0] + LINK_PX + 5;
	arrow_set[2].y_ed = link_y[0] + LINK_PX / 2 + 10;
}

void link_draw() { //����link_arr_rect��arrow_set����ͼ��
	link_node* p = head->next;
	int i;

	cleardevice();
	setfillcolor(EGEARGB(255, 0x66, 0xcc, 0xff));
	setcolor(EGEARGB(0xff, 0x00, 0x00, 0x00));
	setfont(20, 0, "Hack");

	for (i = 0; i < link_node_size; i++) { //�������
		if (link_arr_rect[i].x == 0) continue; //�����δ��ʼ���ķ���
		ege_fillrect(link_arr_rect[i].x, link_arr_rect[i].y, LINK_PX, LINK_PX);
		xyprintf(link_arr_rect[i].x + 5 * (5 - strlen(link_arr_rect[i].str)), link_arr_rect[i].y + 15, "%s", link_arr_rect[i].str);
	}

	for (i = 1; i <= (link_node_size - 1) << 1; i++) { //�����ͷ
		if (arrow_set[i].x_st == 0) continue; //�����δ��ʼ���ļ�ͷ
		draw_arrow(i);
	}

	text_show(2);
}

void link_draw_with_text() { //���pre��nextָ��
	link_draw();
	for (int j = 0; j < text_cnt; j++) if (text_set[j].x != 0) {
		text_show(j);
	}
}

void link_move_back(int loc) {
	Sleep(200);
	double dx = 2; //ÿ֡�ƶ��ľ���
	for (int i = 0; i < 50; i++, delay_fps(60)) { //��֡ѭ��
		for (int j = loc; j < link_node_size; j++) {
			link_arr_rect[j].x += dx;
		}

		arrow_set[(loc - 1) << 1 | 1].x_ed += dx;
		arrow_set[loc << 1].x_st += dx;

		for (int j = loc << 1 | 1; j <= (link_node_size - 1) << 1; j++) {
			arrow_set[j].x_st += dx;
			arrow_set[j].x_ed += dx;
		}

		text_set[1].x += dx;

		link_draw_with_text();
	}
}

void link_insert_rect(int loc, int val) { //���뷽��
	for (int i = link_node_size; i > loc; i--) {
		link_rect_cpy(i, i - 1);
	}
	link_arr_rect[loc].x = 0;
	for (int i = link_node_size << 1; i >= (loc + 1) << 1; i--) {
		arrow_cpy(i, i - 2);
	}
	arrow_set[loc << 1].x_st = 0;
	arrow_set[loc << 1 | 1].x_st = 0;
	link_node_size++; //���ӽڵ���

	color_t fill_color_pre = getfillcolor(), color_pre = getcolor();

	int da = 8, a = 0;

	sprintf(link_arr_rect[loc].str, "%d", val);

	for (int i = 0; i < 32; i++, delay_fps(60)) { //��֡ѭ��,������
		link_draw_with_text();

		setfillcolor(EGEARGB(a, 0x66, 0xcc, 0xff));
		setcolor(EGEARGB(a, 0x00, 0x00, 0x00));

		ege_fillrect(link_x[loc], link_y[loc] - 100, LINK_PX, LINK_PX);
		xyprintf(link_x[loc] + 5 * (5 - strlen(link_arr_rect[loc].str)), link_y[loc] - 100 + 15, "%s", link_arr_rect[loc].str);

		a += da;
	}

	link_arr_rect[loc].x = link_x[loc]; //��ӷ���ʵ��
	link_arr_rect[loc].y = link_y[loc] - 100;

	setfillcolor(fill_color_pre);
	setcolor(color_pre);
}

void link_insert_arrow(int loc) {
	strcpy(text_set[2].str, "p->next = nxt");
	link_draw_with_text();
	Sleep(300);
	arrow_appear(loc << 1 | 1, // loc.next = nxt
				 link_x[loc] + LINK_PX + 5, link_y[loc] - 100 + LINK_PX / 2 - 10, 
				 link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 - 10);

	strcpy(text_set[2].str, "p->pre = pre");
	link_draw_with_text();
	Sleep(300);
	arrow_appear(loc << 1,  //loc.pre = pre
				 link_x[loc] - 10, link_y[loc] - 100 + LINK_PX / 2 + 10, 
				 link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 + 10);

	strcpy(text_set[2].str, "pre->next = p");
	arrow_disappear((loc - 1) << 1 | 1, link_draw_with_text); 
	arrow_appear((loc - 1) << 1 | 1, 
				  link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 - 10, 
				  link_x[loc] - 10, link_y[loc] + LINK_PX / 2 - 10 - 100);
	
	strcpy(text_set[2].str, "nxt->pre = p");
	arrow_disappear((loc + 1) << 1, link_draw_with_text);
	arrow_appear((loc + 1) << 1, 
				  link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 + 10, 
				  link_x[loc] + LINK_PX + 5, link_y[loc] + LINK_PX / 2 + 10 - 100);
}

void link_move_down(int loc) { // ����ڵ�ʱ�������ڵ�����
	int dy = 2;
	for (int i = 0; i < 50; i++, delay_fps(60)) { //��֡ѭ��
		arrow_set[(loc - 1) << 1 | 1].y_ed += dy;
		arrow_set[loc << 1].y_st += dy;
		arrow_set[loc << 1 | 1].y_st += dy;
		arrow_set[(loc + 1) << 1].y_ed += dy;
		link_arr_rect[loc].y += dy;
		link_draw();
	}
}

void link_insert_input_draw() {
	color_t color_pre = getcolor();

	text txt;
	txt.color = BLACK;
	strcpy(txt.font_name, "Hack");
	txt.font_size = 15;
	strcpy(txt.str, "input the insert value:");
	txt.x = 50; 
	txt.y = 385;
	link_draw();
	text_show(txt);
}

void link_insert() {
	link_node* p = (link_node*)malloc(sizeof(link_node)), *pre = head, *nxt = head->next; //����ڵ�ռ�
	if (!p) exit(0);

	link_insert_input_draw();
	p->val = input_box_get();
	if (p->val == -1) { quit_flag = 1; return; };

	text_set[0].x = text_set[1].x = 0;


	text_cpy(3, 0); //text_set[3]Ϊ�����ռ䣬 ���ڽ��������
	text_set[3].x = link_x[0] + 8;
	text_appear(3, link_draw_with_text);

	text_set[0].x = link_x[0] + 8;

	text_cpy(3, 1);
	text_set[3].x = link_x[1] + 8;
	text_appear(3, link_draw_with_text);

	text_set[1].x = link_x[1] + 8;

	text_set[3].x = 0;

	int loc = 1;
	char str[64];

	while (nxt != tail) {
		if (nxt->val >= p->val) break;

		sprintf(str, "nxt->val(%d) < p->val(%d)", nxt->val, p->val);
		strcpy(text_set[2].str, str);

		pre = nxt;
		nxt = nxt->next;
		loc++;
		text_move(0, LINK_PX * 2, 0, 60, link_draw);
		text_move(1, LINK_PX * 2, 0, 60, link_draw);
	}
	if (nxt == tail) strcpy(text_set[2].str, "nxt == tail; break;");
	else {
		sprintf(str, "nxt->val(%d) >= p->val(%d); break;", nxt->val, p->val);
		strcpy(text_set[2].str, str);
	}

	link_move_back(loc);

	p->pre = pre;
	p->next = nxt;
	pre->next = p;
	nxt->pre = p;

	link_insert_rect(loc, p->val);
	link_insert_arrow(loc);
	link_move_down(loc);

	strcpy(text_set[2].str, "");
}

void link_out_info() { // text_set[2]���ڴ������Ϣ
	if (text_set[2].x == 0) return;
	text_show(2);
	Sleep(500);
}

void link_insert_main() {
	link_draw();
	Sleep(500);

	text_update(0, "pre", link_x[0] + 15, link_y[0] + LINK_PX + 15, EGEARGB(255, 0x00, 0x00, 0x00), 20, "Hack");
	text_update(1, "nxt", link_x[1] + 15, link_y[1] + LINK_PX + 30, EGEARGB(255, 0x00, 0x00, 0x00), 20, "Hack");

	if (link_node_size < LINK_MAX_SIZE + 2 && !quit_flag) {
		link_insert();
		link_draw();
		Sleep(300);
	}
}

void link_move_front(int loc) {
	Sleep(200);
	double dx = 2; //ÿ֡�ƶ��ľ���
	for (int i = 0; i < 50; i++, delay_fps(60)) { //��֡ѭ��
		for (int j = loc; j < link_node_size; j++) {
			link_arr_rect[j].x -= dx;
		}

		arrow_set[(loc - 1) << 1 | 1].x_ed -= dx;
		arrow_set[loc << 1].x_st -= dx;

		for (int j = loc << 1 | 1; j <= (link_node_size - 1) << 1; j++) {
			arrow_set[j].x_st -= dx;
			arrow_set[j].x_ed -= dx;
		}

		text_set[1].x -= dx;

		link_draw();
	}
}

void link_delete_rect(int loc) { //ɾ���ڵ� ��link_node_size--��
	char str[64]; //��ʱ���汻ɾ���ڵ����Ϣ
	strcpy(str, link_arr_rect[loc].str);
	int x = link_arr_rect[loc].x, y = link_arr_rect[loc].y;

	for (int i = loc; i < link_node_size; i++) { //ǰ�Ʒ���
		link_rect_cpy(i, i + 1);
	}

	for (int i = loc << 1; i < ((link_node_size - 1) << 1 | 1); i++) { //ǰ�Ƽ�ͷ
		arrow_cpy(i, i + 2);
	}

	link_node_size--; //���ٽڵ���

	color_t fill_color_pre = getfillcolor(), color_pre = getcolor();

	int da = 8, a = 255;

	for (int i = 0; i < 32; i++, delay_fps(60)) { //��֡ѭ��,���񽥳�
		link_draw_with_text();

		setfillcolor(EGEARGB(a, 0x66, 0xcc, 0xff));
		setcolor(EGEARGB(a, 0x00, 0x00, 0x00));

		ege_fillrect(x, y, LINK_PX, LINK_PX);
		ege_drawtext(str, x + 5 * (5 - strlen(str)), y + 15);

		a -= da;
	}

	setfillcolor(fill_color_pre);
	setcolor(color_pre);
}

void link_delete_arrow(int loc) {
	strcpy(text_set[2].str, "p->pre->next = p->next;"); //p->pre->next = p->next
	arrow_disappear((loc - 1) << 1 | 1, link_draw_with_text);
	arrow_appear((loc - 1) << 1 | 1,
				link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 - 10, 
				link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 - 10);

	strcpy(text_set[2].str, "p->next->pre = p->pre;"); // p->next->pre = p->pre
	arrow_disappear((loc + 1) << 1, link_draw_with_text); 
	arrow_appear((loc + 1) << 1,
		link_x[loc + 1] - 10, link_y[loc + 1] + LINK_PX / 2 + 10,
		link_x[loc - 1] + LINK_PX + 5, link_y[loc - 1] + LINK_PX / 2 + 10);
	
	strcpy(text_set[2].str, "free(p);"); //ɾ��p��ǰ��ڵ�
	arrow_disappear(loc << 1, link_draw_with_text);
	arrow_disappear(loc << 1 | 1, link_draw_with_text);
}

void link_move_up(int loc) {
	int dy = -2;
	for (int i = 0; i < 50; i++, delay_fps(60)) { //��֡ѭ��
		arrow_set[(loc - 1) << 1 | 1].y_ed += dy;
		arrow_set[loc << 1].y_st += dy;
		arrow_set[loc << 1 | 1].y_st += dy;
		arrow_set[(loc + 1) << 1].y_ed += dy;
		link_arr_rect[loc].y += dy;
		link_draw_with_text();
	}
}

void link_delete_input_draw() {
	color_t color_pre = getcolor();

	text txt;
	txt.color = BLACK;
	strcpy(txt.font_name, "Hack");
	txt.font_size = 15;
	strcpy(txt.str, "input the delete value:");
	txt.x = 50;
	txt.y = 385;
	link_draw();
	text_show(txt);
}

void link_delete() {
	link_delete_input_draw();

	int val = input_box_get();
	if (val == -1) { quit_flag = 1; return; }

	text_cpy(3, 0);
	text_set[3].x = link_x[1] + 15;

	text_appear(3, link_draw);
	text_set[3].x = 0;

	text_set[0].x = link_x[1] + 15;

	int loc = 1;
	char str[64];
	link_node* p = head->next;

	while (p != tail) {
		if (p->val >= val) break;

		sprintf(str, "p->val(%d) < val(%d)", p->val, val);
		strcpy(text_set[2].str, str);

		p = p->next;
		loc++;
		text_move(0, LINK_PX * 2, 0, 60, link_draw);
	}

	if (p == tail) strcpy(text_set[2].str, "p == tail; break; (unfind)");
	else if (p->val != val) {
		sprintf(str, "p->val(%d) > val; (unfind)", p->val);
		strcpy(text_set[2].str, str);
	}
	else {
		sprintf(str, "p->val(%d) == val(%d); (find)", p->val, val);
		strcpy(text_set[2].str, str);

		Sleep(500);
		link_move_up(loc);

		p->pre->next = p->next;
		p->next->pre = p->pre;
		free(p);

		link_delete_arrow(loc);
		link_delete_rect(loc);
		link_move_front(loc);
	}
}

void link_delete_main() {
	link_draw();

	strcpy(text_set[1].str, "");
	strcpy(text_set[0].str, "p");

	if (link_node_size > 2 && !quit_flag) {
		link_delete();
		link_draw();
		Sleep(300);
	}
}

void link_free() {
	link_node* p = head, * q;
	while (q != nullptr) {
		q = p->next;
		free(p);
		p = q;
	}
}
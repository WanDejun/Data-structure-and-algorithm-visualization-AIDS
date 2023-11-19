//��б�Ǿ�����Ϊ��y������ļн�
#include "cycle_queue.h"
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include "draw_rect.h"
#include "textout.h"
#include "draw_arrow.h"
#include "input_box.h"
#include "random_color.h"
#include "grey.h"
#include "draw_error.h"

typedef struct {
	rect rt; //����
	text txt; //�ı�
	bool visible;
} rect_with_text; //������ַ�

typedef struct {
	color_t col;
	text txt;
	bool visible;
} color_with_text; //��ɫ���ı�

typedef struct {
	int r_st, r_ed;
	double theta;
	int r_txt;
	text txt;
	bool visible;
} arrow_with_text; //��ͷ���ı�

// Բ��x��y����							  ���Ԫ�ظ���				СԲ�뾶	  ��Բ�뾶	�ı��ͼ�ͷĩ�˵�x��y��ֵ
const int center_x = 900, center_y = 360, cycle_queue_max_size = 7, r1 = 150, r2 = 250, text_det_x = 5, text_det_y = 10;

bool cycle_queue_quit_flag; //����˳�

color_with_text cycle_queue_node_set[cycle_queue_max_size + 1]; //�ڵ�����

static arrow_with_text tail_p, head_p; //���ӻ�ָ��

int tail_loc, head_loc;

void cycle_queue_init() { //��ʼ��
	cycle_queue_quit_flag = 0;

	tail_loc = head_loc = 0; 

	for (int i = 0; i <= cycle_queue_max_size; i++) { //��ʼ���ڵ�
		cycle_queue_node_set[i].visible = 0;

		cycle_queue_node_set[i].txt.x = center_x + static_cast<double>(r1 + r2) / 2 * sin(PI * (1.0 / 8 + i * 1.0 / 4)) - text_det_x;
		cycle_queue_node_set[i].txt.y = center_y - static_cast<double>(r1 + r2) / 2 * cos(PI * (1.0 / 8 + i * 1.0 / 4)) - text_det_y;
		
		strcpy(cycle_queue_node_set[i].txt.font_name, "Hack");
	}

	//��ʼ��ͷβָ��
	tail_p.r_st = 80; //��ͷ��ʼ�˾�Բ�ĵİ뾶
	tail_p.r_ed = 140; //��ͷ��ֹ�˾�Բ�ĵİ뾶
	tail_p.theta = PI / (cycle_queue_max_size + 1); //��ͷ��б��
	tail_p.txt.color = BLACK; //��ʼ���ı�
	strcpy(tail_p.txt.font_name, "Hack");
	tail_p.txt.font_size = 30;
	strcpy(tail_p.txt.str, "Tail");
	tail_p.r_txt = 0;
	tail_p.visible = 1; //��Ϊ�ɼ�

	head_p.r_st = 310;
	head_p.r_ed = 260;
	head_p.theta = PI / (cycle_queue_max_size + 1);
	head_p.txt.color = BLACK;
	strcpy(head_p.txt.font_name, "Hack");
	head_p.txt.font_size = 30;
	strcpy(head_p.txt.str, "Head");
	head_p.r_txt = 340;
	head_p.visible = 1;
}

void cycle_queue_draw_tail() { //���βָ��
	tail_p.txt.x = center_x + tail_p.r_txt * sin(tail_p.theta) - 30; //����theta re�����ı�λ�� 30 �� 20 Ϊƫ��������֤������ʾ
	tail_p.txt.y = center_y - tail_p.r_txt * cos(tail_p.theta) - 20;

	static arrow t; //����theta��ʼ����ͷt
	t.x_st = center_x + tail_p.r_st * sin(tail_p.theta);
	t.x_ed = center_x + tail_p.r_ed * sin(tail_p.theta);
	t.y_st = center_y - tail_p.r_st * cos(tail_p.theta);
	t.y_ed = center_y - tail_p.r_ed * cos(tail_p.theta);

	setfillcolor(BLACK); //��ͷ��ɫ
	draw_arrow(t); //���Ƽ�ͷ

	text_show(tail_p.txt); //����ı�
}

void cycle_queue_draw_head() { //���ͷָ�룬��tail����
	head_p.txt.x = center_x + head_p.r_txt * sin(head_p.theta) - 30;
	head_p.txt.y = center_y - head_p.r_txt * cos(head_p.theta) - 20;

	arrow t;
	t.x_st = center_x + head_p.r_st * sin(head_p.theta);
	t.x_ed = center_x + head_p.r_ed * sin(head_p.theta);
	t.y_st = center_y - head_p.r_st * cos(head_p.theta);
	t.y_ed = center_y - head_p.r_ed * cos(head_p.theta);

	setfillcolor(BLACK);
	draw_arrow(t);

	text_show(head_p.txt);
}

void cycle_queue_draw_bk() { //���Ʊ���
	setcolor(BLACK);

	ege_ellipse(center_x - r2, center_y - r2, r2 * 2, r2 * 2); //��������Բ
	ege_ellipse(center_x - r1, center_y - r1, r1 * 2, r1 * 2);

	int x_st, x_ed, y_st, y_ed;

	for (int i = 0; i < cycle_queue_max_size + 1; i++) { //���Ƹ��ӵı߿�
		x_st = center_x + r1 * sin(2 * i * PI / (cycle_queue_max_size + 1));
		x_ed = center_x + r2 * sin(2 * i * PI / (cycle_queue_max_size + 1));
		y_st = center_y - r1 * cos(2 * i * PI / (cycle_queue_max_size + 1));
		y_ed = center_y - r2 * cos(2 * i * PI / (cycle_queue_max_size + 1));
		ege_line(x_st, y_st, x_ed, y_ed);
	}
}

void cycle_queue_draw() { //�������������������
	cleardevice();

	cycle_queue_draw_bk(); //����

	if (tail_p.visible) cycle_queue_draw_tail(); //����ָ��
	if (head_p.visible) cycle_queue_draw_head();

	for (int i = 0; i <= cycle_queue_max_size; i++) { //ÿ���ڵ����
		if (cycle_queue_node_set[i].visible) {
			setfillcolor(cycle_queue_node_set[i].col);
			
			floodfill(cycle_queue_node_set[i].txt.x, cycle_queue_node_set[i].txt.y, BLACK);
			text_show(cycle_queue_node_set[i].txt);
		}
	}
}

void cycle_queue_node_appear(int val) { //���루û��������ege����ò�Ʋ�֧�֣���
	int a = -1, da = 4;

	cycle_queue_node_set[tail_loc].col = rand_color(); //�����ɫ
	int grey = get_grey(cycle_queue_node_set[tail_loc].col); //����Ҷ�

	if (grey < 128) cycle_queue_node_set[tail_loc].txt.color = WHITE; //���ݻҶ���ɫ
	else cycle_queue_node_set[tail_loc].txt.color = BLACK;

	sprintf(cycle_queue_node_set[tail_loc].txt.str, "%d", val);
	if (val >= 10) cycle_queue_node_set[tail_loc].txt.x -= 10;

	cycle_queue_node_set[tail_loc].visible = 1;

	for (int i = 0; i < 64; i++, delay_fps(60)) { //��֡ѭ��
		a += da;

		cycle_queue_node_set[tail_loc].col %= (1ll << 24);
		cycle_queue_node_set[tail_loc].col |= (a << 24);

		cycle_queue_node_set[tail_loc].txt.color %= (1ll << 24);
		cycle_queue_node_set[tail_loc].txt.color |= (a << 24);

		cycle_queue_draw();
	}
}

void cycle_queue_head_move(double det_theta) { //�ƶ�ͷָ�루����ͷָ����theta���ƣ�ֻ��Ҫ�ı�thetaֵ���ɣ�
	double d_theta = det_theta / 60 + 1e-6;
	for (int i = 0; i < 60; i++, delay_fps(60)) {
		head_p.theta += d_theta;

		cycle_queue_draw();
	}
}

void cycle_queue_tail_move(double det_theta) { //����βָ�루��head���գ�
	double d_theta = det_theta / 60 + 1e-6;
	for (int i = 0; i < 60; i++, delay_fps(60)) {
		tail_p.theta += d_theta;

		cycle_queue_draw();
	}
}

void cycle_queue_push() { //���
	cycle_queue_draw(); //���ǻ���һ��

	if ((tail_loc + 1) % (cycle_queue_max_size + 1) == head_loc) { //�ж��Ƿ��пռ�
		draw_error();
		return;
	}

	int val = input_box_get(); //��ȡ����

	if (val == -1) {
		cycle_queue_quit_flag = 1;
		return;
	}

	cycle_queue_node_appear(val); //�����ڵ�
	cycle_queue_tail_move(2 * PI / (cycle_queue_max_size + 1)); //�ƶ�ָ��
	(tail_loc = tail_loc + 1) %= (cycle_queue_max_size + 1); //ָ���±�+1ȡģ
}

void cycle_queue_node_disappear() { //�ڵ�ͼ����ʧ
	int a = 256, da = 4;

	for (int i = 0; i < 64; i++, delay_fps(60)) {
		a -= da;

		cycle_queue_node_set[head_loc].col %= (1ll << 24);
		cycle_queue_node_set[head_loc].col |= (a << 24);

		cycle_queue_node_set[head_loc].txt.color %= (1ll << 24);
		cycle_queue_node_set[head_loc].txt.color |= (a << 24);

		cycle_queue_draw();
	}

	if (cycle_queue_node_set[head_loc].txt.str[1] != '\0') cycle_queue_node_set[head_loc].txt.x += 10;
	cycle_queue_node_set[head_loc].visible = 0;
}

void cycle_queue_pop() { //����
	cycle_queue_draw(); //���ǻ���

	if (head_loc == tail_loc) {
		draw_error();
		return;
	}

	cycle_queue_node_disappear();
	cycle_queue_head_move(2 * PI / (cycle_queue_max_size + 1));
	(head_loc = head_loc + 1) %= (cycle_queue_max_size + 1);
}

void cycle_queue_UI() { // �������� ���Ǵ�ӡ x:(300-450)px y:(400-480, 500-580, 600-680)px
	rect_with_text key[3];
	rect_with_text quit;

	//��ʼ�����������������ʾ
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

	key[0].rt.color = EGEARGB(0, 0, 0, 0); //͸������
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //��
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //��

	key[0].txt.x = 300;

	strcpy(key[0].txt.str, "NEXT STEP:");
	strcpy(key[1].txt.str, "PUSH");
	strcpy(key[2].txt.str, "POP");


	//��ʼ��quit
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

	//���
	for (int i = 0; i < 3; i++) {
		rect_show(key[i].rt);
		text_show(key[i].txt);
	}
	rect_show(quit.rt);
	text_show(quit.txt);
}

void cycle_queue_main() { //��ں���
	cycle_queue_init(); //��ʼ��
	cycle_queue_draw();

	int x, y;
	mouse_msg msg = { 0 };

	cycle_queue_UI();
	for (; is_run() && !cycle_queue_quit_flag; delay_fps(60)) {
		//��ȡ�����Ϣ���˺�������ȴ������к����������
		while (mousemsg()) {
			msg = getmouse();
		}
		if (!msg.is_down()) continue;

		flushmouse(); //�������������

		x = msg.x;
		y = msg.y;

		if (x > 300 && x < 450) {
			if (y > 500 && y < 580) {
				cycle_queue_push();
				cycle_queue_UI();
			}
			else if (y > 600 && y < 680) {
				cycle_queue_pop();
				cycle_queue_UI();
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				cycle_queue_quit_flag = 1;
			}
		}
	}
}
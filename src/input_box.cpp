#include "input_box.h"
#include <graphics.h>
#include <stdlib.h>
#include "draw_rect.h"
#include "textout.h"

//���������� ��50 - 220�� * ��400 - 690��

const int box_size = 50, space_size = 10; 

typedef struct {
	rect rt; //����
	text txt; //�ı�
} rect_with_text; //������ַ�

rect_with_text rect_set[12]; //12������
rect screen; //���Ա���
text input_num[2]; //�������� [0]Ϊ��λ�����ң� [1]Ϊ��λ������

void input_box_init() {
	//rect_set��ʼ��
	for (int i = 0; i < 12; i++) {
		rect_set[i].rt.x = 50 + 60 * (i % 3);
		rect_set[i].rt.y = 460 + 60 * static_cast<int>(i / 3);
		rect_set[i].rt.x_size = 50;
		rect_set[i].rt.y_size = 50;
		rect_set[i].rt.color = EGEARGB(255, 230, 217, 226);
		rect_set[i].txt.color = EGEARGB(255, 0x00, 0x00, 0x00);
		strcpy(rect_set[i].txt.font_name, "Hack");
		rect_set[i].txt.font_size = 20;
		rect_set[i].txt.x = 50 + 60 * (i % 3) + 20;
		rect_set[i].txt.y = 460 + 60 * static_cast<int>(i / 3) + 15;
	}
	strcpy(rect_set[0].txt.str, "7");
	strcpy(rect_set[1].txt.str, "8");
	strcpy(rect_set[2].txt.str, "9");
	strcpy(rect_set[3].txt.str, "4");
	strcpy(rect_set[4].txt.str, "5");
	strcpy(rect_set[5].txt.str, "6");
	strcpy(rect_set[6].txt.str, "1");
	strcpy(rect_set[7].txt.str, "2");
	strcpy(rect_set[8].txt.str, "3");
	strcpy(rect_set[9].txt.str, "0");
	strcpy(rect_set[10].txt.str, "CLR");
	strcpy(rect_set[11].txt.str, "ENTER");
	rect_set[10].txt.x -= 10;
	rect_set[11].txt.x -= 18;
	rect_set[11].txt.font_size = 18;
	rect_set[11].txt.y += 3;
	rect_set[11].rt.color = EGEARGB(255, 0xEF, 0x00, 0x00);

	//screen��ʼ��
	screen.color = EGEARGB(255, 31, 31, 31);
	screen.x = 50;
	screen.y = 400;
	screen.x_size = 170;
	screen.y_size = 50;
	
	//input_num��ʼ��
	for (int i = 0; i < 2; i++) {
		input_num[i].color = EGEARGB(255, 200, 200, 200);
		strcpy(input_num[i].font_name, "Hack");
		input_num[i].font_size = 40;
	}
	strcpy(input_num[0].str, "0");
	strcpy(input_num[1].str, "0");
	input_num[0].x = 190;
	input_num[0].y = 405;
	input_num[1].x = 170;
	input_num[1].y = 405;
}

void input_box_show() { //�������С����
	for (int i = 0; i < 12; i++) {
		rect_show(rect_set[i].rt);
		text_show(rect_set[i].txt);
	}
	rect_show(screen);
	text_show(input_num[0]);
	text_show(input_num[1]);
}

void input_box_show_with(void draw()) { //��draw�ı��������С����
	draw();
	input_box_show();
}

void press_num(int num) {
	input_num[1].str[0] = input_num[0].str[0];
	input_num[0].str[0] = '0' + num;
}

void press_clr() {
	input_num[0].str[0] = '0';
	input_num[1].str[0] = '0';
}

int input_box_get(void draw()) { //��ȡ���룬Ŀǰֻд����������ӻ�С�������룬 �п��˿���дͨ����������Ĵ���
	int x, y, pre_n = -1;
	mouse_msg msg = { 0 };
	double time_pre = fclock(), time;

	for (; is_run(); delay_fps(60)) {
		input_box_show();

		//��ȡ�����Ϣ���˺�������ȴ������к����������
		while (mousemsg()) {
			msg = getmouse();
		}
		time = fclock();
		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (y > 460 && y < 510) {
			if (x > 50 && x < 100 && (pre_n != 7 || time - time_pre >= 0.2)) {
				pre_n = 7;
				press_num(7);
			}
			else if (x > 110 && x < 160 && (pre_n != 8 || time - time_pre >= 0.2)) {
				pre_n = 8;
				press_num(8);
			}
			else if (x > 170 && x < 220 && (pre_n != 9 || time - time_pre >= 0.2)) {
				pre_n = 9;
				press_num(9);
			}
		}
		else if (y > 520 && y < 570) {
			if (x > 50 && x < 100 && (pre_n != 4 || time - time_pre >= 0.2)) {
				pre_n = 4;
				press_num(4);
			}
			else if (x > 110 && x < 160 && (pre_n != 5 || time - time_pre >= 0.2)) {
				pre_n = 5;
				press_num(5);
			}
			else if (x > 170 && x < 220 && (pre_n != 6 || time - time_pre >= 0.2)) {
				pre_n = 6;
				press_num(6);
			}
		}
		else if (y > 580 && y < 630) {
			if (x > 50 && x < 100 && (pre_n != 1 || time - time_pre >= 0.2)) {
				pre_n = 1;
				press_num(1);
			}
			else if (x > 110 && x < 160 && (pre_n != 2 || time - time_pre >= 0.2)) {
				pre_n = 2;
				press_num(2);
			}
			else if (x > 170 && x < 220 && (pre_n != 3 || time - time_pre >= 0.2)) {
				pre_n = 3;
				press_num(3);
			}
		}
		else if (y > 640 && y < 690 && (pre_n != 0 || time - time_pre >= 0.2)) {
			if (x > 50 && x < 100) {
				pre_n = 0;
				press_num(0);
			}
			else if (x > 110 && x < 160 && (pre_n != 10 || time - time_pre >= 0.2)) {
				pre_n = 10;
				press_clr();
			}
			else if (x > 170 && x < 220 && (pre_n != 11 || time - time_pre >= 0.2)) {
				pre_n = 11;
				return (input_num[1].str[0] - '0') * 10 + input_num[0].str[0] - '0';
			}
		}
		time_pre = time;
	}
	return 0;
}
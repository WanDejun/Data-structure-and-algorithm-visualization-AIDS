//倾斜角均定义为与y负方向的夹角
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
	rect rt; //方格
	text txt; //文本
	bool visible;
} rect_with_text; //方格和字符

typedef struct {
	color_t col;
	text txt;
	bool visible;
} color_with_text; //颜色和文本

typedef struct {
	int r_st, r_ed;
	double theta;
	int r_txt;
	text txt;
	bool visible;
} arrow_with_text; //箭头和文本

// 圆心x，y坐标							  最大元素个数				小圆半径	  大圆半径	文本和箭头末端的x，y差值
const int center_x = 900, center_y = 360, cycle_queue_max_size = 7, r1 = 150, r2 = 250, text_det_x = 5, text_det_y = 10;

bool cycle_queue_quit_flag; //标记退出

color_with_text cycle_queue_node_set[cycle_queue_max_size + 1]; //节点数组

static arrow_with_text tail_p, head_p; //可视化指针

int tail_loc, head_loc;

void cycle_queue_init() { //初始化
	cycle_queue_quit_flag = 0;

	tail_loc = head_loc = 0; 

	for (int i = 0; i <= cycle_queue_max_size; i++) { //初始化节点
		cycle_queue_node_set[i].visible = 0;

		cycle_queue_node_set[i].txt.x = center_x + static_cast<double>(r1 + r2) / 2 * sin(PI * (1.0 / 8 + i * 1.0 / 4)) - text_det_x;
		cycle_queue_node_set[i].txt.y = center_y - static_cast<double>(r1 + r2) / 2 * cos(PI * (1.0 / 8 + i * 1.0 / 4)) - text_det_y;
		
		strcpy(cycle_queue_node_set[i].txt.font_name, "Hack");
	}

	//初始化头尾指针
	tail_p.r_st = 80; //箭头起始端距圆心的半径
	tail_p.r_ed = 140; //箭头终止端距圆心的半径
	tail_p.theta = PI / (cycle_queue_max_size + 1); //箭头倾斜角
	tail_p.txt.color = BLACK; //初始化文本
	strcpy(tail_p.txt.font_name, "Hack");
	tail_p.txt.font_size = 30;
	strcpy(tail_p.txt.str, "Tail");
	tail_p.r_txt = 0;
	tail_p.visible = 1; //调为可见

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

void cycle_queue_draw_tail() { //输出尾指针
	tail_p.txt.x = center_x + tail_p.r_txt * sin(tail_p.theta) - 30; //根据theta re计算文本位置 30 和 20 为偏移量，保证居中显示
	tail_p.txt.y = center_y - tail_p.r_txt * cos(tail_p.theta) - 20;

	static arrow t; //根据theta初始化箭头t
	t.x_st = center_x + tail_p.r_st * sin(tail_p.theta);
	t.x_ed = center_x + tail_p.r_ed * sin(tail_p.theta);
	t.y_st = center_y - tail_p.r_st * cos(tail_p.theta);
	t.y_ed = center_y - tail_p.r_ed * cos(tail_p.theta);

	setfillcolor(BLACK); //箭头颜色
	draw_arrow(t); //绘制箭头

	text_show(tail_p.txt); //输出文本
}

void cycle_queue_draw_head() { //输出头指针，与tail对照
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

void cycle_queue_draw_bk() { //绘制背景
	setcolor(BLACK);

	ege_ellipse(center_x - r2, center_y - r2, r2 * 2, r2 * 2); //绘制两个圆
	ege_ellipse(center_x - r1, center_y - r1, r1 * 2, r1 * 2);

	int x_st, x_ed, y_st, y_ed;

	for (int i = 0; i < cycle_queue_max_size + 1; i++) { //绘制格子的边框
		x_st = center_x + r1 * sin(2 * i * PI / (cycle_queue_max_size + 1));
		x_ed = center_x + r2 * sin(2 * i * PI / (cycle_queue_max_size + 1));
		y_st = center_y - r1 * cos(2 * i * PI / (cycle_queue_max_size + 1));
		y_ed = center_y - r2 * cos(2 * i * PI / (cycle_queue_max_size + 1));
		ege_line(x_st, y_st, x_ed, y_ed);
	}
}

void cycle_queue_draw() { //主输出函数（会清屏）
	cleardevice();

	cycle_queue_draw_bk(); //背景

	if (tail_p.visible) cycle_queue_draw_tail(); //两根指针
	if (head_p.visible) cycle_queue_draw_head();

	for (int i = 0; i <= cycle_queue_max_size; i++) { //每个节点输出
		if (cycle_queue_node_set[i].visible) {
			setfillcolor(cycle_queue_node_set[i].col);
			
			floodfill(cycle_queue_node_set[i].txt.x, cycle_queue_node_set[i].txt.y, BLACK);
			text_show(cycle_queue_node_set[i].txt);
		}
	}
}

void cycle_queue_node_appear(int val) { //渐入（没渐起来，ege函数貌似不支持？）
	int a = -1, da = 4;

	cycle_queue_node_set[tail_loc].col = rand_color(); //随机颜色
	int grey = get_grey(cycle_queue_node_set[tail_loc].col); //计算灰度

	if (grey < 128) cycle_queue_node_set[tail_loc].txt.color = WHITE; //根据灰度着色
	else cycle_queue_node_set[tail_loc].txt.color = BLACK;

	sprintf(cycle_queue_node_set[tail_loc].txt.str, "%d", val);
	if (val >= 10) cycle_queue_node_set[tail_loc].txt.x -= 10;

	cycle_queue_node_set[tail_loc].visible = 1;

	for (int i = 0; i < 64; i++, delay_fps(60)) { //逐帧循环
		a += da;

		cycle_queue_node_set[tail_loc].col %= (1ll << 24);
		cycle_queue_node_set[tail_loc].col |= (a << 24);

		cycle_queue_node_set[tail_loc].txt.color %= (1ll << 24);
		cycle_queue_node_set[tail_loc].txt.color |= (a << 24);

		cycle_queue_draw();
	}
}

void cycle_queue_head_move(double det_theta) { //移动头指针（由于头指针由theta控制，只需要改变theta值即可）
	double d_theta = det_theta / 60 + 1e-6;
	for (int i = 0; i < 60; i++, delay_fps(60)) {
		head_p.theta += d_theta;

		cycle_queue_draw();
	}
}

void cycle_queue_tail_move(double det_theta) { //绘制尾指针（与head对照）
	double d_theta = det_theta / 60 + 1e-6;
	for (int i = 0; i < 60; i++, delay_fps(60)) {
		tail_p.theta += d_theta;

		cycle_queue_draw();
	}
}

void cycle_queue_push() { //入队
	cycle_queue_draw(); //覆盖绘制一次

	if ((tail_loc + 1) % (cycle_queue_max_size + 1) == head_loc) { //判断是否有空间
		draw_error();
		return;
	}

	int val = input_box_get(); //获取输入

	if (val == -1) {
		cycle_queue_quit_flag = 1;
		return;
	}

	cycle_queue_node_appear(val); //新增节点
	cycle_queue_tail_move(2 * PI / (cycle_queue_max_size + 1)); //移动指针
	(tail_loc = tail_loc + 1) %= (cycle_queue_max_size + 1); //指针下标+1取模
}

void cycle_queue_node_disappear() { //节点图形消失
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

void cycle_queue_pop() { //出队
	cycle_queue_draw(); //覆盖绘制

	if (head_loc == tail_loc) {
		draw_error();
		return;
	}

	cycle_queue_node_disappear();
	cycle_queue_head_move(2 * PI / (cycle_queue_max_size + 1));
	(head_loc = head_loc + 1) %= (cycle_queue_max_size + 1);
}

void cycle_queue_UI() { // 不清屏， 覆盖打印 x:(300-450)px y:(400-480, 500-580, 600-680)px
	rect_with_text key[3];
	rect_with_text quit;

	//初始化两个按键和输出提示
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

	key[0].rt.color = EGEARGB(0, 0, 0, 0); //透明背景
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //绿
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //蓝

	key[0].txt.x = 300;

	strcpy(key[0].txt.str, "NEXT STEP:");
	strcpy(key[1].txt.str, "PUSH");
	strcpy(key[2].txt.str, "POP");


	//初始化quit
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

	//输出
	for (int i = 0; i < 3; i++) {
		rect_show(key[i].rt);
		text_show(key[i].txt);
	}
	rect_show(quit.rt);
	text_show(quit.txt);
}

void cycle_queue_main() { //入口函数
	cycle_queue_init(); //初始化
	cycle_queue_draw();

	int x, y;
	mouse_msg msg = { 0 };

	cycle_queue_UI();
	for (; is_run() && !cycle_queue_quit_flag; delay_fps(60)) {
		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		if (!msg.is_down()) continue;

		flushmouse(); //清空鼠标输入队列

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
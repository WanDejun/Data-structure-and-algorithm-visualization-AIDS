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

rect_with_text queue_by_link_node_set[queue_by_link_max_size + 1]; //保存节点信息，+ 1是为了在节点在内存空间中前移时方便用+1这个多余节点的信息重置前一个节点

rect_with_text null_node; //null节点

arrow_with_text queue_by_link_head_p, queue_by_link_tail_p;

arrow queue_by_link_arrow_set[queue_by_link_max_size]; //保存箭头信息

text queue_by_link_info;

static int queue_by_link_head_loc, queue_by_link_tail_loc;

void queue_by_link_init() { //初始化
	queue_by_link_quit_flag = 0; //结束标识符置0

	queue_by_link_head_loc = queue_by_link_tail_loc = 0;

	for (int i = 0, x = 350, y = 335; i <= queue_by_link_max_size; i++, x += 2 * queue_by_link_bk_size) { //x起始坐标350
		//节点方格初始化
		queue_by_link_node_set[i].rt.color = EGEARGB(0xff, 0x66, 0xcc, 0xff);
		queue_by_link_node_set[i].rt.x = x;
		queue_by_link_node_set[i].rt.x_size = queue_by_link_bk_size;
		queue_by_link_node_set[i].rt.y = y;
		queue_by_link_node_set[i].rt.y_size = queue_by_link_bk_size;

		//节点文本初始化
		queue_by_link_node_set[i].txt.color = BLACK;
		queue_by_link_node_set[i].txt.font_size = 20;
		strcpy(queue_by_link_node_set[i].txt.font_name, FONT);
		queue_by_link_node_set[i].txt.x = x + text_det_x;
		queue_by_link_node_set[i].txt.y = y + text_det_y;

		//节点可见标识置0
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
	strcpy(null_node.txt.font_name, FONT);
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
	strcpy(queue_by_link_head_p.txt.font_name, FONT);
	queue_by_link_head_p.txt.font_size = 30;
	strcpy(queue_by_link_head_p.txt.str, "Head");
	queue_by_link_head_p.txt.x = queue_by_link_head_p.aw.x_st - 35;
	queue_by_link_head_p.txt.y = queue_by_link_head_p.aw.y_st - 35;

	//初始化tail指针
	queue_by_link_tail_p.aw.x_st = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_tail_p.aw.x_ed = queue_by_link_node_set[0].rt.x + queue_by_link_bk_size / 2;
	queue_by_link_tail_p.aw.y_st = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 50;
	queue_by_link_tail_p.aw.y_ed = queue_by_link_node_set[0].rt.y + queue_by_link_bk_size + 20;
	queue_by_link_tail_p.visible = 1;

	queue_by_link_tail_p.txt.color = BLACK;
	strcpy(queue_by_link_tail_p.txt.font_name, FONT);
	queue_by_link_tail_p.txt.font_size = 30;
	strcpy(queue_by_link_tail_p.txt.str, "Tail");
	queue_by_link_tail_p.txt.x = queue_by_link_tail_p.aw.x_st - 35;
	queue_by_link_tail_p.txt.y = queue_by_link_tail_p.aw.y_st + 4;

	//info初始化
	queue_by_link_info.color = BLACK;
	strcpy(queue_by_link_info.font_name, FONT);
	queue_by_link_info.font_size = 30;
	queue_by_link_info.x = 80;
	queue_by_link_info.y = 80;
	strcpy(queue_by_link_info.str, "");
}

void queue_by_link_draw() { //主绘图函数，包含清屏
	cleardevice(); //清屏

	rect_show(null_node.rt); //绘制null节点
	text_show(null_node.txt);

	for (int i = 0; i <= queue_by_link_max_size; i++) { //绘制节点
		if (queue_by_link_node_set[i].visible) { //可见节点
			rect_show(queue_by_link_node_set[i].rt);
			text_show(queue_by_link_node_set[i].txt);
		}
	}

	setfillcolor(BLACK); //设置箭头颜色

	for (int i = 0; i < queue_by_link_max_size; i++) { //绘制箭头
		if (queue_by_link_arrow_set[i].visible) {
			draw_arrow(queue_by_link_arrow_set[i]);
		}
	}

	if (queue_by_link_head_p.visible) { //绘制头尾指针
		text_show(queue_by_link_head_p.txt);
		draw_arrow(queue_by_link_head_p.aw);
	}
	if (queue_by_link_tail_p.visible) {
		text_show(queue_by_link_tail_p.txt);
		draw_arrow(queue_by_link_tail_p.aw);
	}

	text_show(queue_by_link_info); //输出提示信息
}

void queue_by_link_null_back() {//null节点后移
	double rt_x = null_node.rt.x, txt_x = null_node.txt.x, dx = static_cast<double>(100) / 60 + 1e-6; //节点位置， dx:移动速度

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		rt_x += dx;
		txt_x += dx;

		null_node.rt.x = rt_x;
		null_node.txt.x = txt_x;

		queue_by_link_draw();
	}
}

void queue_by_link_node_appear(int val) { //产生新节点
	double a = 0, da = static_cast<double>(255) / 30 + 1e-6; //透明度即透明度变化速度
	int i_a = 0;

	sprintf(queue_by_link_node_set[queue_by_link_tail_loc].txt.str, "%d", val); //txt.val值修改
	if (val >= 10) queue_by_link_node_set[queue_by_link_tail_loc].txt.x -= 5;

	queue_by_link_node_set[queue_by_link_tail_loc].visible = 1; //新节点调为可见

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

void queue_by_link_tail_back() { //尾指针后移
	double aw_x = queue_by_link_tail_p.aw.x_st, txt_x = queue_by_link_tail_p.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		aw_x += dx;
		txt_x += dx;

		queue_by_link_tail_p.aw.x_st = queue_by_link_tail_p.aw.x_ed = aw_x;
		queue_by_link_tail_p.txt.x = txt_x;
		queue_by_link_draw();
	}
}

void queue_by_link_head_back() {//头指针后移
	double aw_x = queue_by_link_head_p.aw.x_st, txt_x = queue_by_link_head_p.txt.x, dx = static_cast<double>(100) / 60 + 1e-6;

	for (int i = 0; i < 60; i++, delay_fps(60)) {
		aw_x += dx;
		txt_x += dx;

		queue_by_link_head_p.aw.x_st = queue_by_link_head_p.aw.x_ed = aw_x;
		queue_by_link_head_p.txt.x = txt_x;
		queue_by_link_draw();
	}
}

void queue_by_link_push() { //入队
	queue_by_link_draw();

	if (queue_by_link_tail_loc == queue_by_link_max_size) { //超出上限，输出错误信息
		draw_error();
		return;
	}

	int val = input_box_get();

	if (val == -1) { //退出键被按下
		queue_by_link_quit_flag = 1;
		return;
	}

	queue_by_link_null_back(); //null节点后移

	//提示信息，产生新节点，新节点初始化
	if (queue_by_link_tail_loc > 0) strcpy(queue_by_link_info.str, "Tail->next = malloc(sizeof node); Tail->next->next = NULL;");
	else strcpy(queue_by_link_info.str, "Head = Tail = malloc(sizeof node); Tail->next = NULL;");

	queue_by_link_node_appear(val);

	if (queue_by_link_tail_loc != 0) { //入队前队列不为空，尾指针后移
		strcpy(queue_by_link_info.str, "Tail = Tail->next;");
		queue_by_link_tail_back();
	}
	queue_by_link_tail_loc++;
}

void queue_by_link_node_disappear() {
	double a = 255, da = static_cast<double>(255) / 30 + 1e-6; //a为透明度 da为淡化速录
	int i_a = 0;

	queue_by_link_arrow_set[queue_by_link_head_loc].visible = 0;

	for (int i = 0; i < 30; i++, delay_fps(60)) { //逐帧循环
		a -= da;
		i_a = a;

		queue_by_link_node_set[queue_by_link_head_loc].rt.color %= (1 << 24); //重置透明度值
		queue_by_link_node_set[queue_by_link_head_loc].rt.color |= (i_a << 24);

		queue_by_link_node_set[queue_by_link_head_loc].txt.color %= (1 << 24);
		queue_by_link_node_set[queue_by_link_head_loc].txt.color |= (i_a << 24);

		queue_by_link_draw();

		setfillcolor(EGEARGB(i_a, 0x00, 0x00, 0x00)); //重置箭头透明度
		draw_arrow(queue_by_link_arrow_set[queue_by_link_head_loc]);
	}

	queue_by_link_node_set[queue_by_link_head_loc].visible = 0;
}

void queue_by_link_move_front() {
	int dx = 2; //位移速度

	for (int i = 0; i < 50; i++, delay_fps(60)) { //逐帧循环
		for (int i = 0; i < queue_by_link_tail_loc - 1; i++) {
			queue_by_link_arrow_set[i].x_ed -= dx;
			queue_by_link_arrow_set[i].x_st -= dx;

			queue_by_link_node_set[i].rt.x -= dx;
			queue_by_link_node_set[i].txt.x -= dx;
		}


		null_node.rt.x -= dx; //null节点前移
		null_node.txt.x -= dx; 

		if (queue_by_link_tail_loc > 1) { //队列不为空时，头尾指针前移
			queue_by_link_head_p.aw.x_st -= dx; //头
			queue_by_link_head_p.aw.x_ed -= dx;
			queue_by_link_head_p.txt.x -= dx;

			queue_by_link_tail_p.aw.x_st -= dx; //尾
			queue_by_link_tail_p.aw.x_ed -= dx;
			queue_by_link_tail_p.txt.x -= dx;
		}

		queue_by_link_draw(); //每帧绘图
	}
}

void queue_by_link_pop() { //出队
	queue_by_link_draw();

	if (queue_by_link_tail_loc <= 0) { //尾指针为零表示队列中无元素，输出error错误信息
		draw_error();
		return;
	}

	strcpy(queue_by_link_info.str, "Head = Head->next;"); //提示信息输出，头指针后移

	if (queue_by_link_tail_loc != 1) queue_by_link_head_back(); //原本队列不为空时，头指针后移

	strcpy(queue_by_link_info.str, "free();"); //输出提示信息，释放原来的节点

	queue_by_link_node_disappear();

	for (int i = 0; i < queue_by_link_tail_loc - 1; i++) { // 节点在内存空间中前移
		queue_by_link_node_set[i].rt.x = queue_by_link_node_set[i + 1].rt.x;
		queue_by_link_node_set[i].visible = queue_by_link_node_set[i + 1].visible;

		text_cpy(&queue_by_link_node_set[i].txt, queue_by_link_node_set[i + 1].txt); //节点文本信息复制
	}

	queue_by_link_node_set[queue_by_link_tail_loc - 1].visible = 0; //最后一个节点重置
	if (queue_by_link_node_set[queue_by_link_tail_loc - 1].txt.str[1] != '\0')
		queue_by_link_node_set[queue_by_link_tail_loc - 1].txt.x += 5;

	for (int i = 0; i < queue_by_link_tail_loc - 1; i++) { //箭头前移
		queue_by_link_arrow_set[i].x_st += 100;
		queue_by_link_arrow_set[i].x_ed += 100;
		queue_by_link_arrow_set[i].visible = queue_by_link_arrow_set[i + 1].visible;
	}

	queue_by_link_arrow_set[queue_by_link_tail_loc - 1].visible = 0; //最后一个箭头重置

	queue_by_link_move_front(); //整体前移动画

	queue_by_link_tail_loc--;
}

void queue_by_link_UI() {
	rect_with_text key[3] = { 0 };
	rect_with_text quit = { 0 };

	//初始化两个按键和输出提示
	for (int i = 0; i < 3; i++) {
		key[i].rt.x = 300; //x:300
		key[i].rt.y = 400 + i * 100; //y: 400 - 
		key[i].rt.x_size = 150;
		key[i].rt.y_size = 80;

		key[i].txt.color = EGEARGB(0xff, 0x00, 0x00, 0x00);
		strcpy(key[i].txt.font_name, FONT);
		key[i].txt.font_size = 35; //35号字体
		key[i].txt.x = key[i].rt.x + 32;
		key[i].txt.y = key[i].rt.y + 24;
	}
	key[2].txt.x += 10;

	key[0].rt.color = EGEARGB(0, 0, 0, 0); //透明背景
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //绿
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //蓝

	key[0].txt.y = 470;
	key[0].txt.x = 300;
	key[0].txt.font_size = 25;

	strcpy(key[0].txt.str, "NEXT STEP:");
	strcpy(key[1].txt.str, "PUSH");
	strcpy(key[2].txt.str, "POP");


	//初始化quit
	strcpy(quit.txt.str, "QUIT");
	quit.txt.color = EGEARGB(255, 0x200, 0x200, 0x200);
	quit.txt.font_size = 22;
	strcpy(quit.txt.font_name, FONT);
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

void queue_by_link_main() {
	queue_by_link_init();
	queue_by_link_draw();

	int x, y;
	mouse_msg msg = { 0 };

	for (; is_run() && !queue_by_link_quit_flag; delay_fps(60)) {
		queue_by_link_UI();

		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		flushmouse(); //清空鼠标输入队列

		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (x > 300 && x < 450) {
			if (y > 500 && y < 580) {
				queue_by_link_push();
			}
			else if (y > 600 && y < 680) {
				queue_by_link_pop();
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				queue_by_link_quit_flag = 1;
			}
		}
	}

}
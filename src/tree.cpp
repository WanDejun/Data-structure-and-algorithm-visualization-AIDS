
#include "tree_demo.h"
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include "typedef.h"

static const int max_size = 15, det_x = 5, det_y = 8, x_space = 100, y_space = 80, x_mid = 640;

static cycle_with_text node_set[max_size + 2];

static arrow tree_arrow_set[max_size + 2];

static cycle marked_cycle;

static int size, info_len;

static double dx_arr[max_size + 2], x_arr[max_size + 2];

static text info;

static bool tree_quit_flag;

static void init() {
	tree_quit_flag = 0; //退出标识置0

	size = 0; //节点大小

	//初始化节点
	for (int i = 1; i <= max_size + 1; i++) {
		node_set[i].cyc.color = EGERGB(0x39, 0xcc, 0xbb); //初音绿
		node_set[i].cyc.r = 20;

		node_set[i].txt.color = BLACK;
		strcpy(node_set[i].txt.font_name, FONT);
		node_set[i].txt.font_size = 15;

		node_set[i].visible = 0;
	}
	node_set[1].cyc.x = x_mid; //初始化头结点
	node_set[1].cyc.y = 200;
	node_set[1].txt.x = node_set[1].cyc.x - det_x;
	node_set[1].txt.y = node_set[1].cyc.y - det_y;

	//初始化arrow_set
	for (int i = 1; i <= max_size + 1; i++) {
		tree_arrow_set[i].visible = 0;
	}

	//初始化heap_info
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
	text_show(info);
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
	设此时节点数为n， 则共有log(n) + 1层
	第log(n) + 1层共有n个节点
	第i层到底层的距离为log(n) + 1 - i
	则每个第i层节点对应2 ^ (log(n) + 1 - i)个节点，即第i层节点间距为x_space * 2 ^ (log(n) + 1 - i)
	原本间距为x_space * 2 ^ (log(n) - i)，即间距翻倍
	校准时，第i层节点与（i - 1）层的偏移量也为 x_space * 2 ^ (log(n) - i)
	*/
	for (int i = 0; i <= max_size; i++) {
		dx_arr[i] = 0;
		x_arr[i] = node_set[i].cyc.x;
	}

	int f = 100;
	int m = log(size) / log(2) + 1e-6 + 1;

	for (int i = 1; i < m - 1; i++) {
		for (int loc = 1 << i; loc < (1 << (i + 1)); loc++) { // 第i + 1层
			if (loc & 1) dx_arr[loc] += (double(x_space) * double(pow(2, m - (i + 1) - 1)) / 2) / f;
			else dx_arr[loc] -= (double(x_space) * double(pow(2, m - (i + 1) - 1)) / 2) / f;
			push_down(loc);
		}
	}

	for (int i = 0; i < f; i++, delay_fps(60)) {
		for (int j = 2; j < size; j++) {
			x_arr[j] += dx_arr[j];

			node_set[j].cyc.x = x_arr[j];

			if (j & 1) { //右节点
				tree_arrow_set[j].x_st = x_arr[j >> 1] + 15;
				tree_arrow_set[j].x_ed = x_arr[j] - 20;
			}
			else { //左节点
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
		for (int loc = 1 << i; loc < (1 << (i + 1)); loc++) { // 第i + 1层
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

	if ((size & (-size)) == size && size >= 4) { //判断是否为2的倍数
		stretch();
	}

	appear();
}

static void marked_cycle_flash() {
	color_t color_pre = marked_cycle.color;
	Sleep(200);

	marked_cycle.color = RED;
	cycle_show(marked_cycle);
	Sleep(200);

	marked_cycle.color = color_pre;
}

static void marked_cycle_move(int st, int ed) {
	//从下标为st的节点上移动至下标为ed的节点上

	int f = 50;
	double x = node_set[st].cyc.x + 0.5, y = node_set[st].cyc.y + 0 / 5;
	double dx = double(node_set[ed].cyc.x - x) / f, dy = double(node_set[ed].cyc.y - y) / f;

	for (int i = 0; i < f; i++, delay_fps(60)) {
		x += dx; 
		y += dy;

		marked_cycle.x = x;
		marked_cycle.y = y;
		
		draw();
		cycle_show(marked_cycle);
	}

}

static void preorder_traversal_sub(int loc) { //preorder_traversal的子程序
	marked_cycle_flash();// 闪红圈提示

	//info插入
	if (info_len != 2) info.str[info_len - 1] = ',';
	else info_len = 1;
	for (int i = 0; node_set[loc].txt.str[i] != '\0'; i++, info_len++) {
		info.str[info_len] = node_set[loc].txt.str[i];
	}
	info.str[info_len++] = ']';
	info.str[info_len] = '\0';

	node_set[loc].cyc.color = EGERGB(196, 216, 242); //甘雨浅蓝

	if ((loc << 1) <= max_size) {//遍历左孩子
		marked_cycle_move(loc, loc << 1); 
		preorder_traversal_sub(loc << 1);
	}
	if ((loc << 1 | 1) <= max_size) {//遍历右节点
		marked_cycle_move(loc, loc << 1 | 1);
		preorder_traversal_sub(loc << 1 | 1);
	}
	node_set[loc].cyc.color = EGERGB(0x39, 0xcc, 0xbb); //初音绿
	
	if (loc != 1) marked_cycle_move(loc, loc >> 1);
}

static void inorder_traversal_sub(int loc) { //inorder_traversal的子程序
	node_set[loc].cyc.color = EGERGB(196, 216, 242); //甘雨浅蓝

	if ((loc << 1) <= max_size) {//遍历左孩子
		marked_cycle_move(loc, loc << 1);
		inorder_traversal_sub(loc << 1);
	}

	marked_cycle_flash(); // 闪红圈提示

	//info插入
	if (info_len != 2) info.str[info_len - 1] = ',';
	else info_len = 1;
	for (int i = 0; node_set[loc].txt.str[i] != '\0'; i++, info_len++) {
		info.str[info_len] = node_set[loc].txt.str[i];
	}
	info.str[info_len++] = ']';
	info.str[info_len] = '\0';

	if ((loc << 1 | 1) <= max_size) {//遍历右节点
		marked_cycle_move(loc, loc << 1 | 1);
		inorder_traversal_sub(loc << 1 | 1);
	}
	node_set[loc].cyc.color = EGERGB(0x39, 0xcc, 0xbb); //初音绿

	if (loc != 1) marked_cycle_move(loc, loc >> 1);
}

static void postorder_traversal_sub(int loc) { //postorder_traversal的子程序
	node_set[loc].cyc.color = EGERGB(196, 216, 242); //甘雨浅蓝

	if ((loc << 1) <= max_size) { //遍历左孩子
		marked_cycle_move(loc, loc << 1);
		postorder_traversal_sub(loc << 1);
	}

	if ((loc << 1 | 1) <= max_size) { //遍历右节点
		marked_cycle_move(loc, loc << 1 | 1);
		postorder_traversal_sub(loc << 1 | 1);
	}

	marked_cycle_flash(); //闪红圈提示

	//info插入
	if (info_len != 2) info.str[info_len - 1] = ',';
	else info_len = 1;
	for (int i = 0; node_set[loc].txt.str[i] != '\0'; i++, info_len++) {
		info.str[info_len] = node_set[loc].txt.str[i];
	}
	info.str[info_len++] = ']';
	info.str[info_len] = '\0';

	node_set[loc].cyc.color = EGERGB(0x39, 0xcc, 0xbb); //初音绿

	if (loc != 1) marked_cycle_move(loc, loc >> 1);
}

static void traversal(int opcode) { // 0: 前序；1：中序； 2：后序
	strcpy(info.str, "[]");
	info_len = 2;

	cycle_cpy(&marked_cycle, node_set[1].cyc); //marked_cycle放置到根节点上
	marked_cycle.color = EGERGB(93, 116, 162); //甘雨蓝
	cycle_show(marked_cycle);

	switch (opcode) {//根节点调用子程序
	case 0:
		preorder_traversal_sub(1); //前序
		break;
	case 1:
		inorder_traversal_sub(1); //中序
		break;
	case 2:
		postorder_traversal_sub(1); //后序
		break;
	default:
		break;
	}

	draw(); //用于覆盖掉marked_cycle
	strcpy(info.str, ""); //info清空在draw（）后为了保留输出，方便查看
}

static void UI() {
	rect_with_text key[3];
	rect_with_text quit;

	//初始化两个按键和输出提示
	for (int i = 0; i < 3; i++) {
		key[i].rt.x = 290 + 250 * i;
		key[i].rt.y = 600;
		key[i].rt.x_size = 200;
		key[i].rt.y_size = 50;

		key[i].txt.color = EGEARGB(0xff, 0x00, 0x00, 0x00);
		strcpy(key[i].txt.font_name, FONT);
		key[i].txt.font_size = 20;
		key[i].txt.x = key[i].rt.x + 15;
		key[i].txt.y = key[i].rt.y + 15;
	}

	key[0].rt.color = EGEARGB(128, 200, 60, 60); //红
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //绿
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //蓝

	strcpy(key[0].txt.str, "preorder traversal");
	strcpy(key[1].txt.str, "inorder travesal");
	strcpy(key[2].txt.str, "postorder traversal");


	//初始化quit
	strcpy(quit.txt.str, "QUIT");
	quit.txt.color = EGEARGB(255, 0x200, 0x200, 0x200);
	quit.txt.font_size = 22;
	strcpy(quit.txt.font_name, FONT);
	quit.txt.x = 1180 + 13;
	quit.txt.y = 20 + 19;

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

void tree_main() {
	init();
	draw();

	for (int i = 0; i < max_size; i++) { //建树
		insert();
	}

	int x, y;

	for (; is_run() && !tree_quit_flag; delay_fps(60)) {
		UI();

		mouse_msg msg = { 0 };

		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg()) {
			msg = getmouse();
		}
		flushmouse(); //清空鼠标输入队列

		if (!msg.is_down()) continue;

		x = msg.x;
		y = msg.y;

		if (y > 600 && y < 650) {
			if (x > 290 && x < 490) {
				traversal(0); //前序
			}
			else if (x > 540 && x < 740) {
				traversal(1); //中序
			}
			else if (x > 790 && x < 990) {
				traversal(2); //后序
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				tree_quit_flag = 1;
			}
		}
	}
}
#include <graphics.h>
#include "link.h"
#include "link_insert_delete.h"
#include "textout.h"
#include "typedef.h"
#include "draw_rect.h"

extern bool link_quit_flag;

void link_UI() { // 不清屏， 覆盖打印 x:(300-450)px y:(400-480, 500-580, 600-680)px
	rect_with_text key[3];
	rect_with_text quit;

	//初始化两个按键和输出提示
	for (int i = 0; i < 3; i++) {
		key[i].rt.x = 300;
		key[i].rt.y = 400 + i * 100;
		key[i].rt.x_size = 150;
		key[i].rt.y_size = 80;

		key[i].txt.color = EGEARGB(0xff, 0x00, 0x00, 0x00);
		strcpy(key[i].txt.font_name, FONT);
		key[i].txt.font_size = 35;
		key[i].txt.x = key[i].rt.x + 16;
		key[i].txt.y = key[i].rt.y + 24;
	}
	key[0].rt.color = EGEARGB(0, 0, 0, 0); //透明背景
	key[1].rt.color = EGEARGB(128, 80, 200, 80); //绿
	key[2].rt.color = EGEARGB(128, 32, 128, 192); //蓝

	key[0].txt.x = 300;

	strcpy(key[0].txt.str, "NEXT STEP:");
	strcpy(key[1].txt.str, "INSERT");
	strcpy(key[2].txt.str, "DELETE");


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

void link_main() {
	link_init();

	int x, y;
	mouse_msg msg = { 0 };

	for (; is_run() && !link_quit_flag; delay_fps(60)) {
		link_UI();

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
				link_insert_main();
				link_UI();
			}
			else if (y > 600 && y < 680) {
				link_delete_main();
				link_UI();
			}
		}
		else if (y > 20 && y < 70) {
			if (x > 1180 && x < 1260) {
				link_free();
				link_quit_flag = 1;
			}
		}
	}
}
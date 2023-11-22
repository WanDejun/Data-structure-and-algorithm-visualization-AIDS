#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include "link.h"
#include "input_box.h"
#include "stack.h"
#include "stack_by_link.h"
#include "cycle_queue.h"
#include "draw_error.h"
#include "queue_by_link.h"
#include "typedef.h"
#include "heap.h"
#include "tree.h"
#include "tree_demo.h"

rect_with_text key[4][4] = { 0 }; //按键（功能）个数

text title;

void draw_test() { ; }

void init() {
    //初始化为640*480大小
    initgraph(1280, 720, INIT_RENDERMANUAL); // 画幅 1280 * 720，关闭自动刷新
    setbkmode(TRANSPARENT); //文本透明背景
    setbkcolor(EGERGB(0xff, 0xff, 0xff)); //设置背景为白色
    srand(time(0)); //初始化随机数种子
    ege_enable_aa(true); // 开启抗锯齿
    input_box_init(); //初始化输入控制台
    setlinewidth(4); //初始化线型
    draw_error_init();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            key[i][j].rt.x = 150 + 260 * j; //起始点坐标（150，320）
            key[i][j].rt.y = 320 + 90 * i; //x方向间距260， y方向间距90
            key[i][j].rt.x_size = 200;
            key[i][j].rt.y_size = 60;
            key[i][j].rt.color = EGEARGB(128, 100, 100, 100);

            key[i][j].txt.color = BLACK;
            strcpy(key[i][j].txt.font_name, "Hack");
            key[i][j].txt.font_size = 18;
            key[i][j].txt.x = key[i][j].rt.x + 5;
            key[i][j].txt.y = key[i][j].rt.y + 12;
        }
    }
    strcpy(key[0][0].txt.str, "");
    strcpy(key[0][1].txt.str, "");
    strcpy(key[0][2].txt.str, "Link: \nInsert & Delete");
    strcpy(key[0][3].txt.str, "Stack: \nArray Inplamentation");

    strcpy(key[1][0].txt.str, "");
    strcpy(key[1][1].txt.str, "");
    strcpy(key[1][2].txt.str, "Heap: \n(priority queue)");
    strcpy(key[1][3].txt.str, "Stack: \nList Inplamentation");

    strcpy(key[2][0].txt.str, "");
    strcpy(key[2][1].txt.str, "");
    strcpy(key[2][2].txt.str, "");
    strcpy(key[2][3].txt.str, "Queue(cycle): \nArray Inplamentation");

    strcpy(key[3][0].txt.str, "");
    strcpy(key[3][1].txt.str, "");
    strcpy(key[3][2].txt.str, "");
    strcpy(key[3][3].txt.str, "Queue: \nList Inplamentation");

    title.color = BLUE;
    strcpy(title.font_name, FONT);
    title.font_size = 60;
    strcpy(title.str, "数据结构与算法可视化辅助程序");
    title.x = 220;
    title.y = 150;
}

void main_UI() { //简易版UI
    cleardevice();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rect_show(key[i][j].rt);
            text_show(key[i][j].txt);
        }
    }

    text_show(title);
}

void trap(int x, int y) {
    if (x > 150 && x < 350) {
        if (y > 320 && y < 380) {
            
        }
        else if (y > 410 && y < 470) {
            
        }
        else if (y > 500 && y < 560) {
            
        }
        else if (y > 590 && y < 650) {
            
        }
    }
    else if (x > 410 && x < 610) {
        if (y > 320 && y < 380) {

        }
        else if (y > 410 && y < 470) {

        }
        else if (y > 500 && y < 560) {

        }
        else if (y > 590 && y < 650) {
            
        }
    }
    else if (x > 670 && x < 870) {
        if (y > 320 && y < 380) {
            link_main();
        }
        else if (y > 410 && y < 470) {
            heap_main();
        }
        else if (y > 500 && y < 560) {

        }
        else if (y > 590 && y < 650) {
        
        }
    }
    else if (x > 930 && x < 1030) {
        if (y > 320 && y < 380) {
            stack_main();
        }
        else if (y > 410 && y < 470) {
            stack_by_link_main();
        }
        else if (y > 500 && y < 560) {
            cycle_queue_main();
        }
        else if (y > 590 && y < 650) {
            queue_by_link_main();
        }
    }
}

int main() {
    init();

    tree_demo_main();

    //int x, y;

    //for (; is_run(); delay_fps(60)) {
    //    main_UI();
    //    
    //    mouse_msg msg = { 0 };
    //    //msg.x = msg.y = 0;

    //    //获取鼠标消息，此函数不会等待，运行后会立即返回
    //    while (mousemsg()) {
    //        msg = getmouse();
    //    }
    //    flushmouse(); //清空鼠标输入队列

    //    if (!msg.is_down()) continue;

    //    x = msg.x;
    //    y = msg.y;

    //    trap(x, y);
    //    flushmouse(); //清空鼠标输入队列
    //}

    //等待用户按键
    getch();

    //关闭图形界面
    closegraph();
    return 0;
}
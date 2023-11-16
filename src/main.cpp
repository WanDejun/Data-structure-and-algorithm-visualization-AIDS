#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include "link.h"
#include "input_box.h"
using namespace std;

void draw_test() { ; }

int main()
{
    //初始化为640*480大小
    initgraph(1280, 720, INIT_RENDERMANUAL); // 画幅 1280 * 720，关闭自动刷新
    setbkmode(TRANSPARENT); //文本透明背景
    setbkcolor(EGERGB(0xff, 0xff, 0xff)); //设置背景为白色
    srand(time(0)); //初始化随机数种子
    ege_enable_aa(true); // 开启抗锯齿
    input_box_init();


    link_main();

    //等待用户按键
    getch();

    //关闭图形界面
    closegraph();
    return 0;
}
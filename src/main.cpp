#include <graphics.h>
using namespace std;

int main()
{
    //初始化为640*480大小
    initgraph(1280, 720, INIT_RENDERMANUAL);
    ege_enable_aa(true);


    setbkcolor(EGERGB(0xff, 0xff, 0xff));

    setfillcolor(EGEARGB(128, 0x66, 0xcc, 0xff));

    bar(100, 100, 140, 140);
    //等待用户按键
    getch();

    //关闭图形界面
    closegraph();
    return 0;
}
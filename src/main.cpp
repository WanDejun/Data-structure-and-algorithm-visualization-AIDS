#include <graphics.h>
using namespace std;

int main()
{
    //��ʼ��Ϊ640*480��С
    initgraph(1280, 720, INIT_RENDERMANUAL);
    ege_enable_aa(true);


    setbkcolor(EGERGB(0xff, 0xff, 0xff));

    setfillcolor(EGEARGB(128, 0x66, 0xcc, 0xff));

    bar(100, 100, 140, 140);
    //�ȴ��û�����
    getch();

    //�ر�ͼ�ν���
    closegraph();
    return 0;
}
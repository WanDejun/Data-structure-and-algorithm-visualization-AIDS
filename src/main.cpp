#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include "link.h"
using namespace std;

int main()
{
    //��ʼ��Ϊ640*480��С
    initgraph(1280, 720, INIT_RENDERMANUAL); // ���� 1280 * 720���ر��Զ�ˢ��
    setbkmode(TRANSPARENT); //�ı�͸������
    setbkcolor(EGERGB(0xff, 0xff, 0xff)); //���ñ���Ϊ��ɫ
    srand(time(0)); //��ʼ�����������
    ege_enable_aa(true); // ���������

    link_main();

    //�ȴ��û�����
    getch();

    //�ر�ͼ�ν���
    closegraph();
    return 0;
}
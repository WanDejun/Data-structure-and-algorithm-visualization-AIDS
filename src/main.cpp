#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include "link.h"
#include "input_box.h"
using namespace std;

void draw_test() { ; }

int main()
{
    //��ʼ��Ϊ640*480��С
    initgraph(1280, 720, INIT_RENDERMANUAL); // ���� 1280 * 720���ر��Զ�ˢ��
    setbkmode(TRANSPARENT); //�ı�͸������
    setbkcolor(EGERGB(0xff, 0xff, 0xff)); //���ñ���Ϊ��ɫ
    srand(time(0)); //��ʼ�����������
    ege_enable_aa(true); // ���������
    input_box_init();


    link_main();

    //�ȴ��û�����
    getch();

    //�ر�ͼ�ν���
    closegraph();
    return 0;
}
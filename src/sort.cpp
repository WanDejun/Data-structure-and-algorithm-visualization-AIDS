#include <stdio.h>
#include <graphics.h>//����EGE��ͷ�ļ�
struct Squire
{
	int zhi;
	int x1;
	int y1;
	int x2;
	int y2;
}squire[10];
void Swap(int x, int y)
{
	int temp = x;     //�����м���������ڽ���������ֵ���м�ý��
	x = y;
	y = temp;
}

static void initsquire()//��ʼ����
{
	squire[0].zhi = 9;
	squire[1].zhi = 20;
	squire[2].zhi = 1;
	squire[3].zhi = 14;
	squire[4].zhi = 18;
	squire[5].zhi = 5;
	squire[6].zhi = 25;
	squire[7].zhi = 10;
	squire[8].zhi = 3;
	squire[9].zhi = 12;
	squire[0].x1 = 200;
	squire[1].x1=224;
	squire[2].x1=248;
	squire[3].x1=272;
	squire[4].x1=296;
	squire[5].x1=320;
	squire[6].x1=342;
	squire[7].x1=366;
	squire[8].x1=390;
	squire[9].x1=414;
	squire[0].x2=224;
	squire[1].x2=248;
	squire[2].x2=272;
    squire[3].x2=296;
	squire[4].x2=320;
	squire[5].x2=342;
	squire[6].x2=366;
	squire[7].x2=390;
	squire[8].x2=414;
	squire[9].x2=438;
	for (int i = 0; i < 10; i++)
	{
		
		squire[i].y1 = 280;
		squire[i].y2 = 280-squire[i].zhi * 10;
	}
}
static void drawsquire()
{
	setfillcolor(GREEN);
	for (int i = 0; i < 10; i++)
	{
		bar(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
		rectangle(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);

	}
	setfont(15, 0, "��Բ");
	//(����߶�,������(Ϊ0��Ϊ����Ӧ),����)��������Ϊ25,��Բ����
	outtextxy(squire[0].x1, 290, "09");
	outtextxy(squire[1].x1, 290, "20");
	outtextxy(squire[2].x1, 290, "01");
	outtextxy(squire[3].x1, 290, "14");
	outtextxy(squire[4].x1, 290, "18");
	outtextxy(squire[5].x1, 290, "05");
	outtextxy(squire[6].x1, 290, "25");
	outtextxy(squire[7].x1, 290, "10");
	outtextxy(squire[8].x1, 290, "03");
	outtextxy(squire[9].x1, 290, "12");
}
 void InsertSort()
{
	 int temp1 = 0,temp2=0;

	 setcolor(EGERGB(0xFF, 0x0, 0x0));	//���û滭��ɫΪ��ɫ

	 setbkcolor(WHITE);					//���ñ�����ɫΪ��ɫ
	 cleardevice();

	 initsquire();
	 drawsquire();

	for (int i = 1; i < 10; i++)
	{
		int flag =squire[i].zhi;   //��¼arr[i] ��ֵ
		for (int j = i - 1; j >= 0 && squire[j].zhi > flag; j-- , delay_fps(1))  //��arr[i]ǰ���һ�����ֱ�a[i]��ʱ
		{ 
			squire[j+1].zhi = squire[j].zhi;  //��arr[i] ����ǰһ�����ֽ��н���
			squire[j].zhi = flag;

			temp2 = squire[j+1].y2;
			squire[j+1].y2 = squire[j].y2;
			squire[j].y2 = temp2;

			temp1 = squire[j+1].y1;
			squire[j+1].y1 = squire[j].y1;
			squire[j].y1 = temp1;

			cleardevice();

			char str[5];
			for (int i = 0; i < 10; i++) {
				sprintf_s(str, "%d", squire[i].zhi);
				outtextxy(squire[i].x1, 290, str);
			}

			for (int i = 0; i < 10; i++)
			{
				bar(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
				rectangle(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
			}
		}
	}
	getch();					//��ͣ���ȴ����̰���	
}

void BubbleSort()
{
	int j, i, tem,temp1,temp2;

	setcolor(EGERGB(0xFF, 0x0, 0x0));	//���û滭��ɫΪ��ɫ

	setbkcolor(WHITE);					//���ñ�����ɫΪ��ɫ
	cleardevice();

	initsquire();
	drawsquire();

	for (i = 0; i < 9; i++)//size-1����Ϊ�������Լ��Ƚϣ����Աȵ�������һ��
	{
		int count = 0;
		for (j = 0; j <9 - i; j++, delay_fps(1))	//size-1-i����Ϊÿһ�˾ͻ���һ�����Ƚ�
		{
			if (squire[j].zhi > squire[j + 1].zhi)//���������ŷ���ǰһ�����ͺ�һ�����Ƚϣ����ǰ���������һ������λ��
			{
				tem =squire[j].zhi;
				squire[j].zhi = squire[j + 1].zhi;
				squire[j + 1].zhi = tem;
				//squire[j + 1].y2 = 280 - squire[j + 1].zhi * 10;
				//squire[j].y2 = 280 - squire[j].zhi * 10;
				temp2 = squire[j + 1].y2;
				squire[j + 1].y2 = squire[j].y2;
				squire[j].y2 = temp2;

				temp1 = squire[j + 1].y1;
				squire[j + 1].y1 = squire[j].y1;
				squire[j].y1 = temp1;
				cleardevice();
				char str[5];
				for (int i = 0; i < 10; i++) {
					sprintf_s(str, "%d", squire[i].zhi);
					outtextxy(squire[i].x1, 290, str);
				}
				for (int i = 0; i < 10; i++)
				{
					bar(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
					rectangle(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
				}
				count = 1;

			}
		}
		if (count == 0)			//���ĳһ��û�н���λ�ã���˵���Ѿ��ź���ֱ���˳�ѭ��
			break;
	}
	getch();					//��ͣ���ȴ����̰���
}
void SelectSort()
{
	int i, j, flag = 0, exchange = 0,temp1,temp2;

	setcolor(EGERGB(0xFF, 0x0, 0x0));	//���û滭��ɫΪ��ɫ

	setbkcolor(WHITE);					//���ñ�����ɫΪ��ɫ
	cleardevice();

	initsquire();
	drawsquire();

	for (i = 0; i < 10; i++)
	{
		flag = i;   //��¼�±�
		for (j = i + 1; j < 10; j++, delay_fps(5))
		{
			if (squire[flag].zhi >= squire[j].zhi)  //flag��j�±�Ƚϣ���flagָ����С����
			{
				flag = j;
			}
		}
		if (flag != i)  //�±겻ͬʱ(��ʼ����)
		{
			exchange = squire[flag].zhi;
			squire[flag].zhi = squire[i].zhi;
			squire[i].zhi = exchange;
			//squire[flag].y2 = 280 - squire[flag].zhi * 10;
			//squire[i].y2 = 280 - squire[i].zhi * 10;
			temp2 = squire[i].y2;
			squire[i].y2 = squire[flag].y2;
			squire[flag].y2 = temp2;

			temp1 = squire[i].y1;
			squire[i].y1 = squire[flag].y1;
			squire[flag].y1 = temp1;
			cleardevice();
			char str[5];
			for (int i = 0; i < 10; i++) {
				sprintf_s(str, "%d", squire[i].zhi);
				outtextxy(squire[i].x1, 290, str);
			}
			for (int i = 0; i < 10; i++)
			{
				bar(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
				rectangle(squire[i].x1, squire[i].y1, squire[i].x2, squire[i].y2);
			}
		}
	}
	getch();					//��ͣ���ȴ����̰���
}

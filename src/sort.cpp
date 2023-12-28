#include <stdio.h>
#include <graphics.h>//包含EGE的头文件
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
	int temp = x;     //定义中间变量，用于交换两个数值的中间媒介
	x = y;
	y = temp;
}

static void initsquire()//初始矩形
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
	setfont(15, 0, "幼圆");
	//(字体高度,字体宽度(为0即为自适应),字形)设置字体为25,幼圆字体
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

	 setcolor(EGERGB(0xFF, 0x0, 0x0));	//设置绘画颜色为红色

	 setbkcolor(WHITE);					//设置背景颜色为白色
	 cleardevice();

	 initsquire();
	 drawsquire();

	for (int i = 1; i < 10; i++)
	{
		int flag =squire[i].zhi;   //记录arr[i] 的值
		for (int j = i - 1; j >= 0 && squire[j].zhi > flag; j-- , delay_fps(1))  //当arr[i]前面的一个数字比a[i]大时
		{ 
			squire[j+1].zhi = squire[j].zhi;  //将arr[i] 和其前一个数字进行交换
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
	getch();					//暂停，等待键盘按键	
}

void BubbleSort()
{
	int j, i, tem,temp1,temp2;

	setcolor(EGERGB(0xFF, 0x0, 0x0));	//设置绘画颜色为红色

	setbkcolor(WHITE);					//设置背景颜色为白色
	cleardevice();

	initsquire();
	drawsquire();

	for (i = 0; i < 9; i++)//size-1是因为不用与自己比较，所以比的数就少一个
	{
		int count = 0;
		for (j = 0; j <9 - i; j++, delay_fps(1))	//size-1-i是因为每一趟就会少一个数比较
		{
			if (squire[j].zhi > squire[j + 1].zhi)//这是升序排法，前一个数和后一个数比较，如果前数大则与后一个数换位置
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
		if (count == 0)			//如果某一趟没有交换位置，则说明已经排好序，直接退出循环
			break;
	}
	getch();					//暂停，等待键盘按键
}
void SelectSort()
{
	int i, j, flag = 0, exchange = 0,temp1,temp2;

	setcolor(EGERGB(0xFF, 0x0, 0x0));	//设置绘画颜色为红色

	setbkcolor(WHITE);					//设置背景颜色为白色
	cleardevice();

	initsquire();
	drawsquire();

	for (i = 0; i < 10; i++)
	{
		flag = i;   //记录下标
		for (j = i + 1; j < 10; j++, delay_fps(5))
		{
			if (squire[flag].zhi >= squire[j].zhi)  //flag和j下标比较，让flag指向最小的数
			{
				flag = j;
			}
		}
		if (flag != i)  //下标不同时(开始交换)
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
	getch();					//暂停，等待键盘按键
}

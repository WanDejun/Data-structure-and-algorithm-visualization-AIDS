//用于输出文本内容的控制，text_set为文本集合

#pragma once
#include <graphics.h>
#ifndef TEXTOUT
#define TEXTOUT

typedef struct TEXT {
	char str[64];
	int x, y;
	color_t color;
	int font_size;
	char font_name[16];
} text;

extern text text_set[32];
extern int text_cnt;

int s2i(char* str); 
void text_init();
void text_cpy(text* u, text v);
void text_cpy(int u, int v);
void text_update(int loc, char str[], int x, int y, color_t color, int font_size, char font_name[]);
void text_show(text txt);
void text_show(int loc);
void text_appear(int loc, void draw());
void text_move(int loc, int dx, int dy, int f, void draw(), bool show_other_text = 1);

#endif // !TEXTOUT

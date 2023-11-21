#pragma once
#include "draw_rect.h"
#include "draw_arrow.h"
#include "textout.h"
#include "cycle.h"

#ifndef TYPEDEF_H
#define TYPEDEF_H

#define FONT ("consolas")

typedef struct {
	rect rt; //����
	text txt; //�ı�
	bool visible;
} rect_with_text; //������ַ�

typedef struct {
	arrow aw; // ��ͷ
	text txt; //�ı�
	bool visible;
} arrow_with_text; //��ͷ���ַ�

typedef struct {
	cycle cyc;
	text txt;
	bool visible;
} cycle_with_text; //Բ���ı�


#endif // !TYPEDEF_H

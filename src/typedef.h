#pragma once
#include "draw_rect.h"
#include "draw_arrow.h"
#include "textout.h"

#ifndef TYPEDEF_H
#define TYPEDEF_H

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

#endif // !TYPEDEF_H

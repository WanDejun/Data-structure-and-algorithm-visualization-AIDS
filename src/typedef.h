#pragma once
#include "draw_rect.h"
#include "draw_arrow.h"
#include "textout.h"
#include "cycle.h"

#ifndef TYPEDEF_H
#define TYPEDEF_H

#define FONT ("consolas")

typedef struct {
	rect rt; //方格
	text txt; //文本
	bool visible;
} rect_with_text; //方格和字符

typedef struct {
	arrow aw; // 箭头
	text txt; //文本
	bool visible;
} arrow_with_text; //箭头和字符

typedef struct {
	cycle cyc;
	text txt;
	bool visible;
} cycle_with_text; //圆和文本


#endif // !TYPEDEF_H

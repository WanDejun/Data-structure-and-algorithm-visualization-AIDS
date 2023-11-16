#pragma once
#include <graphics.h>

#ifndef DRAW_RECT_H
#define DRAW_RECT_H

typedef struct {
	int x, y;
	int x_size, y_size;
	color_t color;
} rect;

void rect_show(rect rt);

#endif // !DRAW_RECT_H

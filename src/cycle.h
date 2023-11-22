#pragma once
#ifndef CYCLE_H
#define CYCLE_H
#include <graphics.h>

typedef  struct {
	color_t color;
	int x;
	int y, r;
} cycle;

void cycle_show(cycle cyc);

void cycle_cpy(cycle *cyc1, const cycle cyc2);

#endif // !CYCLE_H

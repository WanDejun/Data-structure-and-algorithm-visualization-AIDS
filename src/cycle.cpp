#include "cycle.h"
#include <graphics.h>

void cycle_show(cycle cyc) {
	setcolor(cyc.color);
	ege_ellipse(cyc.x - cyc.r, cyc.y - cyc.r, cyc.r * 2, cyc.r * 2);
}

void cycle_cpy(cycle cyc1, const cycle cyc2) {
	cyc1.color = cyc2.color;
	cyc1.r = cyc2.r;
	cyc1.x = cyc2.x;
	cyc1.y = cyc2.y;
}

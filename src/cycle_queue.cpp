#include "cycle_queue.h"
#include <graphics.h>
#include <math.h>
#include "draw_rect.h"
#include "textout.h"
#include "draw_arrow.h"
#include "input_box.h"
#include "random_color.h"
#include "grey.h"

typedef struct {
	color_t col;
	text txt;
	bool visible;
} color_with_text; //颜色和文本

typedef struct {
	arrow aw;
	text txt;
	bool visible;
} arrow_with_text; //箭头和文本

const int center_x = 900, center_y = 360, cycle_queue_max_size = 7, r1 = 150, r2 = 250;

bool cycle_queue_quit_flag;

color_with_text cycle_queue_node_set[cycle_queue_max_size + 1];

arrow_with_text tail_p, head_p;

void init() {
	cycle_queue_quit_flag = 0;

	//for ()
}

void cycle_queue_draw_bk() {
	setcolor(BLACK);

	circle(center_x, center_y, r2);
	circle(center_x, center_y, r1);


}

void cycle_queue_main() {
	cycle_queue_draw_bk();
}
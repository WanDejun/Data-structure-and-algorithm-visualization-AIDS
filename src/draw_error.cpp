#include "draw_error.h"
#include "textout.h"
#include "typedef.h"
#include <graphics.h>

text error_txt;

void draw_error_init() {
	error_txt.color = RED;
	strcpy(error_txt.font_name, FONT);
	error_txt.font_size = 60;
	strcpy(error_txt.str, "ERROR!");
	error_txt.x = 120;
	error_txt.y = 80;
}

void draw_error() {
	text_show(error_txt);
}
#include "random_color.h"

color_t rand_color() { //随机颜色
	return EGEARGB(255, rand() % 255, rand() % 255, rand() % 255);
}
color_t rand_acolor() { //随机颜色和透明度
	return EGEARGB(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}
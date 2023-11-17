#include "random_color.h"

inline color_t rand_color() { //随机颜色
	return EGEARGB(255, rand() % 255, rand() % 255, rand() % 255);
}
inline color_t rand_acolor() { //随机颜色和透明度
	return EGEARGB(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}
#include "random_color.h"

color_t rand_color() { //�����ɫ
	return EGEARGB(255, rand() % 255, rand() % 255, rand() % 255);
}
color_t rand_acolor() { //�����ɫ��͸����
	return EGEARGB(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}
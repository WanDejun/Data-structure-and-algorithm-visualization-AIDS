#include "random_color.h"

inline color_t rand_color() { //�����ɫ
	return EGEARGB(255, rand() % 255, rand() % 255, rand() % 255);
}
inline color_t rand_acolor() { //�����ɫ��͸����
	return EGEARGB(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
}
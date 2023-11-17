#include "grey.h"

inline int get_grey(color_t col) {
	int grey = 500; //������������
	static int arr[3] = { 299, 587, 114 };
	for (int i = 0; i < 3; i++) {
		grey += arr[i] * col % (1 << 8);
		col >> 8;
	}
	grey /= 1000;
}
#include <stdio.h>

struct vector {
	int x, y;
};

struct vector v_2bai(struct vector v);

int main(void)
{
	struct vector a = {1, 2}, b;
	printf("a = (%d, %d)\n", a.x, a.y);
	b = v_2bai(a);
	printf("b = (%d, %d)\n", b.x, b.y);
}

struct vector v_2bai(struct vector v)
{
	v.x *= 2;
	v.y *= 2;
	return v;	
}

/*
a = (1, 2)
b = (2, 4)
*/
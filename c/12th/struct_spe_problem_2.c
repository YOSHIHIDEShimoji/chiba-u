#include <stdio.h>

struct vector
{
	int x, y;
} a, b;

int InnerProduct(struct vector v1, struct vector v2);

int main(void)
{
	int result; 
	a.x = 2;
	a.y = 5;
	b.x = -3;
	b.y = 6;
	result = InnerProduct(a, b);
	printf("Inner product of a and b = %d\n", result);
	return 0;
}

int InnerProduct(struct vector v1, struct vector v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

/*
Inner product of a and b = 24
*/
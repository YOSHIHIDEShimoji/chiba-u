#include <stdio.h>

struct xydate
{
	int x, y;
};

void seki(struct xydate date);

int main(void)
{
	struct xydate hoge;
	hoge.x = 10;
	hoge.y = 11;
	seki(hoge);
	return 0;
}

void seki(struct xydate date)
{
	printf("%d\n", date.x * date.y);
}
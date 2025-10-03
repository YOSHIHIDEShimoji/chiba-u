#include <stdio.h>

struct xydata
{
	int x, y;
};

void seki(struct xydata data);

int main(void)
{
	struct xydata hoge;
	hoge.x = 10;
	hoge.y = 11;
	seki(hoge);
	return 0;
}

void seki(struct xydata data)
{
	printf("%d\n", data.x * data.y);
}

/*
110
*/
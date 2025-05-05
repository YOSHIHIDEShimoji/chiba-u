#include <stdio.h>

int main(void)
{
	int i = 0, j = 0;
	if (i && (j = j + 1)) { ;}
	printf("%d, %d\n", i, j);

	i = 1, j = 0;
	if (i && (j = j + 1)) { ;}
	printf("%d, %d\n", i, j);
	
	return 0;
}

/*
Cは0をfalse、0以外をtrue とみなす。&& は左から評価され、左辺が false の場合は右辺を評価しないため、i = 0 のときはtrueだから、右辺の j = j + 1 が実行されず、i = 0, j = 0となる。
一方、i = 1 のときはtrueだから、 j = j + 1 が実行され、i = 1, j = 1となる。

C treats 0 as false and anything other than 0 as true. && evaluates from left to right, and if the left side is false, the right side is not evaluated. Therefore, when i = 0, the right side j = j + 1 is not executed, and i = 0, j = 0.
On the other hand, when i = 1, the condition is true, so j = j + 1 is executed, resulting in i = 1 and j = 1.
*/


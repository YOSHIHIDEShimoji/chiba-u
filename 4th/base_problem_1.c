#include <stdio.h>

int main(void)
{
	int a = 0, b = 3, c =2;
	a = b = c;
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	printf("c = %d\n", c);

	return 0;
}

/*
'='は右辺から左辺への代入演算子であり、まず、b = c が実行され、c = 2 だから、bの値によらずbには2が代入される。
次に、a = bが実行され、b = 2 だからaの値によらずaには2が代入される。よって実行結果は a = b = c = 2である。

'=' is the assignment operator from the right-hand side to the left-hand side. First, b = c is executed, and c = 2, so b is assigned 2 regardless of the value of b.
Next, a = b is executed, and b = 2, so a is assigned 2 regardless of the value of a.Therefore, the result is a = b = c = 2.
Translated by DeepL
*/

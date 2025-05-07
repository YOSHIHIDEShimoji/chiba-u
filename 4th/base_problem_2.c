#include <stdio.h>

int main(void)
{
	int a, b;
	
	/*
	a = 1, b = 0;
	((a = b) = 2 + 1);
	printf("No.1\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);
	*/

	a = 1, b = 0;
	a = (b = 2) + 1;
	printf("No.2\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);

	a = 1, b = 0;
	a = b = 2 + 1;
	printf("No.3\n");
	printf("a = %d\n", a);
	printf("b = %d\n", b);

	return 0;
}

/*
No.1 is an error.

/home/yshimoji/under_git_clone/4th/base_problem_2.c:8:18: error: lvalue required as left operand of assignment
    8 |         ((a = b) = 2 + 1);
      |                  ^

No.2
a = 3
b = 2
No.3
a = 3
b = 3
*/

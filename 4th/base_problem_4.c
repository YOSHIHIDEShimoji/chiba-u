#include <stdio.h>

int main(void)
{
	int a, b;
	char c;
	double e;
	a = b = 2;
	b += 2;		/* No.1 */
	c = 95;
	b += c;
	b++;		/* No.2 */
	b += c;		/* No.3 */
	printf("%d\n", b++);
	printf("%d\n", b);
	e = b;
	printf("%f\n", e);
	
	return 0;
}

/*
No.1
b = 4

No.2
b = 5 

No.3
b = 195

195
196
196.000000
*/

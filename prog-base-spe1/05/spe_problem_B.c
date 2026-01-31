#include <stdio.h>

int main(void)
{
	int m;
	
	do {
		scanf("%d", &m);
		switch (m) {
			case 3:
			case 4:
			case 5:		printf("Spring\n");	break;
			case 6:
			case 7:
			case 8:		printf("Summer\n");	break;
			case 9:
			case 10:
			case 11:	printf("Autumn\n"); break;
			case 12:
			case 1:
			case 2:		printf("Winter\n");	break;
		}
	} while (m != 99);

	return 0;
}
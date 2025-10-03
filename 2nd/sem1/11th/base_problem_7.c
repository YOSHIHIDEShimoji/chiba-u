#include <stdio.h>

void draw(int n, int i);

int main(void)
{
	int n, i, j;
	printf("ピラミッドの段数を入力して下さい.: ");
	scanf("%d", &n);

	for (i = 1; i <= n; i++) {
		/*for (j = 0; j < n - i; j++) {
			printf(" ");
		}
		for (j = 0; j < i * 2 - 1; j++) {
			printf("*");
		}
		printf("\n");
		*/
		draw(n, i);
	}
}

void draw(int n, int i)
{
	int j;
	for (j = 0; j < n - i; j++) {
		printf(" ");
	}
	for (j = 0; j < i * 2 - 1; j++) {
		printf("*");
	}
	printf("\n");
}

/*
ピラミッドの段数を入力して下さい.: 7
      *
     ***
    *****
   *******
  *********
 ***********
*************
*/

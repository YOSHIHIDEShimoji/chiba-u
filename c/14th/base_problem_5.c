#include <stdio.h>

int main(void)
{
	int s[335];
	for (int i = 0; i <= 335; i++) {
		s[i] = i;
		if (i % 3 == 0) {
			s[i] = 3;
		} if () {

		}

	}
	




	for (int i = 0; i <= 335; i++) {
		printf("%d: %d\n", i, s[i]);
	}
	return 0;
}
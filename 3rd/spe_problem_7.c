#include <stdio.h>

int main(void)
{
	char name[] = {'S', 'h', 'i', 'm', 'o', 'j', 'i', '?', '\0'};

	int i;
	for(i = 0; i <= 8; i++){
	printf("%c = %d\n", name[i], name[i]);
	}
	return 0;
}

/*
name = Shimoji?
*/

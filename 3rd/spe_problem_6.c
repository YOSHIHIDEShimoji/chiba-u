# include <stdio.h>
int main(void)
{
	char name[] = {'S', 'h', 'i', 'm', 'o', 'j', 'i', '\?', '\0'};
	printf("name = %s\n", name);
	return 0;
}

/*
name = Shimoji?
*/

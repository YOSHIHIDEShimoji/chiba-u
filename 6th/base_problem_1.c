#include <stdio.h>

int main(void) 
{
	char ss1[80], ss2[80];
	
	printf("input string 1:");
	gets(ss1);
	printf("input string 2");
	gets(ss2);

	printf("%s", ss1);
	printf("%s\n",ss2);
}

/*
input string 1:asdf
input string 2jkl;
asdfjkl;
*/

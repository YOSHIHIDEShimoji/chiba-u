#include <stdio.h>

void tokyo(void);
void nagoya(void);
void kyoto(void);

int main()
{
	printf("I'm in Chiba.\n");
	tokyo();
	printf("I'm in Chiba.\n");
	nagoya();
	printf("Im in Chiba.\n");
	kyoto();
	printf("I'm in Chiba.\n");
}

void tokyo()
{
	printf("I'm in Tokyo.\n");
}
void nagoya()
{
	printf("I'm in Nagoya.\n");
}
void kyoto()
{
	printf("I'm in Kyoto.\n");
}

/*
I'm in Chiba.
I'm in Tokyo.
I'm in Chiba.
I'm in Nagoya.
Im in Chiba.
I'm in Kyoto.
I'm in Chiba.
*/
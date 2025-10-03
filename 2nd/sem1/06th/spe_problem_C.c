#include <stdio.h>

void tokyo(void);
void nagoya(void);
void kyoto(void);

int main()
{
	printf("I'm in Chiba.\n");
	tokyo();
	printf("Back in Chiba.\n");
}

void tokyo(void)
{
	printf("I'm in Tokyo.\n");
	nagoya();
	printf("Back in Tokyo.\n");
}

void nagoya(void)
{
	printf("I'm in Nagoya.\n");
	kyoto();
	printf("Back in Nagoya.\n");
}

void kyoto(void)
{
	printf("I'm in Kyoto.\n");
}

/*
I'm in Chiba.
I'm in Tokyo.
I'm in Nagoya.
I'm in Kyoto.
Back in Nagoya.
Back in Tokyo.
Back in Chiba.
*/
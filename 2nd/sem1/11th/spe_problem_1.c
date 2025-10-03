#include <stdio.h>

int main(void)
{
	char s[] = "medsys", *ptr = s;
	printf("配列の先頭アドレス = %p\n", ptr);
	printf("学科 = %s\n", s);
	return 0;
}

/*
配列の先頭アドレス = 0x7ffed1627371
学科 = medsys
*/
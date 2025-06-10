#include <stdio.h>
double warikan(int num, int guest, int total);

int main(void) 
{
	int num = 40, guest = 6, total = 780000;
	double each; 
	each = warikan(num, guest, total);
	printf("%7.2f\n", each);
	return 0;
}

double warikan(int num, int guest, int total)
{
	double price;
	price = (double)total / (double)(num - guest);
	return price;
}

/*
22941.18
*/

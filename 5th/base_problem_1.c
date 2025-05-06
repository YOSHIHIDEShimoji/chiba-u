#include <stdio.h>

int main(void)
{
    int i = 1, sum = 0, product = 1;
    do {
        sum += i;
        product *= i;
    } while (++i <= 5);
        printf("sum = %d, product = %d\n", sum, product);    
}
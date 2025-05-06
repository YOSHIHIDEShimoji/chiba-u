#include <stdio.h>

int main(void)
{
    int n = -1;
    char a[] = "prog", b[10];
    do {
        ++n;
        b[n] = a[n];
    } while (b[n] != '\0');
    printf("a = %s b = %s\n", a, b);
    return 0;
}
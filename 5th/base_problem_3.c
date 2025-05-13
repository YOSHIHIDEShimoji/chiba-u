#include <stdio.h>

int main(void)
{
    int n, i;
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        if (n == i) {
            printf("%d\n", i);
        }
        else if (n % i == 0) {
            printf("%d, ", i);
        }
    }
    return 0;
}

/*
360
1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24, 30, 36, 40, 45, 60, 72, 90, 120, 180, 360
*/

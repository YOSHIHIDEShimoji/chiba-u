#include <stdio.h>

int main(void)
{
    int a[4][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
        {10, 11, 12}};

    int i = 0;
        while (i < 4) {
        int j = 0;
        while (j < 3) {
            printf("%3d", a[i][j]);
            j++;
        }
        i++;
        printf("\n");
    }
}
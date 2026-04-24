/* 名称 pd03b-code
 * 説明：ソースコード整形課題の動作検証用コード
 */

#include <stdio.h>

#define N 6


void average(int a[], int n);

int main() {
    int vals[N] = {5, 0, 3, 9, 2, 7};

    printf("numbers: ");
    for (int i=0; i < N; i++) {
        printf("%d ", vals[i]);
    }
    printf("\n");
    average(vals, N);

    return 0;
}


void average(int a[], int n)
{
int b,c;   c=0;
for(b=0;b<n;b++){c+=a[b];}
printf("{average} = %.2f\n",(float)c/n);
}

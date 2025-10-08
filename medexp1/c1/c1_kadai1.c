#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 256

int main()
{
    /* open file */
    FILE *fp = fopen("c1_data.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* define N */
    int N;
    char buf[BUF_SIZE];
    fgets(buf, BUF_SIZE - 1, fp);
    sscanf(buf, "%d\n", &N);

    /* read data */
    int etime = 0, resp = 0;
    int i;
    for (i = 0; i < N; i ++) {
        fgets(buf, BUF_SIZE - 1, fp);
        sscanf(buf, "%d,%d\n", &etime, &resp);

        /* print */
        if (0 <= i && i < 3) {
            printf("%dms:%d\n", etime, resp);
        }
        if (N - 3 <= i && i < N) {
            printf("%dms:%d\n", etime, resp);
        }
    }
    
    /* ending */
    fclose(fp);
}

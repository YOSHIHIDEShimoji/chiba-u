#include <stdio.h>
#include <stdlib.h>

#define N 2000
#define BUF_SIZE 256

int main()
{
    /* open file */
    FILE *fp = fopen("c1_data.txt", "r");
    if (fp == NULL) {
        printf("Can't open data file.\n");
        return 1;
    }

    /* read data */
    char buf[BUF_SIZE];
    int etime = 0, resp = 0;
    int i;
    for (i = 0; i < N; i ++) {
        fgets(buf, BUF_SIZE-1, fp);
        sscanf(buf, "%d,%d\n", &etime, &resp);
        printf("%dms:%d\n", etime, resp);
    }

    /* ending */
    fclose(fp);
}

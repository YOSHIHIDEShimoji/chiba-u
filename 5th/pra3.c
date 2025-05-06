#include <stdio.h>

int main(void)
{
    char c = 'x';
    switch(c) {
        case 'a':
            printf("I'm in case a.\n");
            break;
        case 'x':
            printf("I'm in case x.\n");
            break;
        case 'z':
            printf("I'm in case z.\n");
            break;
        default:
            printf("I'm in default.\n");
    }
}

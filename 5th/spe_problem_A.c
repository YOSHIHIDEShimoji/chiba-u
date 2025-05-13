#include <stdio.h>

int main(void)
{
    char c = 'a';
    switch (c) {
	case 'a':
	    printf("a");
        case 'b':
	    printf("b");
	    break;
	case 'y':	
	    printf("y");
	case 'z':	
	    printf("z");
	    break;
	default: printf("x");
	}
	return 0;
}

/*
x = 'a';
ab

x = 'b';
b

x = 'c';
x
*/

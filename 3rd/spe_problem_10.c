#include <stdio.h>
#include <string.h>

int main(void)
{
	char name2[] = "Yoshihide\tShimoji";
	printf("name = %s\n", name2);

	strcpy(name2, "Yoshihide\nShimoji");
	printf("%s\n", name2);
	return 0;
}

/*
name = Yoshihide        Shimoji
Yoshihide
Shimoji
*/

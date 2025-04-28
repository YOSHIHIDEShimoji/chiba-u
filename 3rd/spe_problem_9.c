# include <stdio.h>

int main(void)
{
	char name2[] = "Yoshihide\tShimoji";
	printf("name = %s\n", name2);

	name2[9] = '\n';
	printf("%s\n", name2);
	return 0;
}

/*
name = Yoshihide        Shimoji
Yoshihide
Shimoji
*/

# include <stdio.h>
main()
{
	char name2[]="Daiki\tEndou";
	printf("name=%s\n",name2);
	name2[5]='\n';
	printf("%s\n",name2);
}

# include <stdio.h>
main()
{
	unsigned char a[8]={1,2,4,8,16,32,64,128};
	printf("%u\n",a[0]);
	printf("%u\n",a[1]);
	printf("%u\n",a[2]);
	printf("%u\n",a[3]);
	printf("%u\n",a[4]);
	printf("%u\n",a[5]);
	printf("%u\n",a[6]);
	printf("%u\n",a[7]);
	printf("%u\n",a[0]);
	printf("%u\n",a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6]+a[7]);
}

#!/bin/bash

# curl -o ~/daiki.sh https://gist.githubusercontent.com/YOSHIHIDEShimoji/b7865b936374739e2e84d6db3d058fa3/raw/daiki.sh
# chmod +x ~/daiki.sh
# bash ~/daiki.sh

# ファイルを作成してそこに移動
mkdir ~/3rd; cd ~/3rd

cat << EOF > work1.c
# include <stdio.h>
main()
{
	char str[] = "This is a string!";
	printf("%c\n",str[0]);
	printf("%c\n",str[16]);
	printf("%c\n",str[17]);
}
EOF

cat << EOF > work2.c
# include <stdio.h>
main()
{
	double a[3]={1.23,2.34,3.45};
	printf("%f\n",a[0]+a[1]+a[2]);
}
EOF

cat << EOF > task1.c
# include <stdio.h>
main()
{
	char a=127;
	unsigned char b=200;
	short c=300;
	int d=123456;
	float e=1234.5;
	long f=12345678901234;
	printf("%d\n",a);
	printf("%u\n",b);
	printf("%d\n",c);
	printf("%d\n",d);
	printf("%f\n",e);
	printf("%ld\n",f);
}
EOF

cat << EOF > task3.c
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
EOF

cat << EOF > task6.c
# include <stdio.h>
main()
{
	char name[] = {'E','n','d','o','u','\?','\0'};
	printf("name = %s\n", name);
}
EOF

cat << EOF < task7.c
# include <stdio.h>
main()
{
	char name[] = {'E','n','d','o','u','\?','\0'};
	printf("%c=%d\n",name[0],name[0]);
	printf("%c=%d\n",name[1],name[1]);
	printf("%c=%d\n",name[2],name[2]);
	printf("%c=%d\n",name[3],name[3]);
	printf("%c=%d\n",name[4],name[4]);
	printf("%c=%d\n",name[5],name[5]);
	printf("%c=%d\n",name[6],name[6]);
}
EOF

cat << EOF > task8.c
# include <stdio.h>
main()
{
	char name2[] = "Daiki\tEndou";
	printf("name=%s\n",name2);
}
EOF

cat << EOF > task9.c
# include <stdio.h>
main()
{
	char name2[]="Daiki\tEndou";
	printf("name=%s\n",name2);
	name2[5]='\n';
	printf("%s\n",name2);
}
EOF

#すべてのファイルをコンパイラ
for f in *.c ; do
	cc $f -o ${f%.c}
done



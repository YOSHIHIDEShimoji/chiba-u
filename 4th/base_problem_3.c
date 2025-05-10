int main(void)
{
	int i = 0, j = 0;
	if (i && (j = j + 1)) { ;}
	printf("%d, %d\n", i, j);

	i = 1, j = 0;
	if (i && (j = j + 1)) { ;}
	printf("%d, %d\n", i, j);
	
	return 0;
}

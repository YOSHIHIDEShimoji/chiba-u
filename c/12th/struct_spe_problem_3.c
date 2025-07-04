#include <stdio.h>

struct person {
	char name[10];
	struct person *partner;
	struct person *child;
	struct person *mother;
} p0 = {"Taro", NULL, NULL, NULL},
  p1 = {"Hanako", NULL, &p0, NULL},
  p2 = {"Hiroshi", &p1, &p0, NULL};

int main(void)
{
	p1.partner = &p2;
	p0.mother = &p1;
	printf("Mother's name = %s\n", p0.mother->name);
	printf("Father's name = %s\n", p0.mother->partner->name);
}

/*
Mother's name = Hanako
Father's name = Hiroshi
*/
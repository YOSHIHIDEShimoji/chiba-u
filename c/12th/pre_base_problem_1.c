#include <stdio.h>
#include <math.h>

#define PI4o3 (4 * M_PI / 3)
#define CUBE(n) ((n)* (n) * (n))

int main(void)
{ 
	double r;
	for (r = 1.0; r <= 10.0; r += 1.0) {
		printf("r = %f の球の体積: %f\n", r, PI4o3 * CUBE(r));
	}
	return 0;
}
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv[]) {

int steps = atoi(argv[1]);

float intervalo = 1.0 / steps;

//realizamos la sumatoria

float sum = 0.0;

for(int i = 0; i < steps; i++)
{
	float a = (i+0.5) * intervalo;

	sum += 4.0 / ( 1.0 + a * a);
}

sum = sum * intervalo;

printf("PI = %f\n", sum);

return 0;
}

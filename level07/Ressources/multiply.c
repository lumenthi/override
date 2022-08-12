#include <stdio.h>
#include <stdlib.h>

unsigned int multiplyWith4(int n)
{
	return (unsigned int)(n << 2);
}
 
int main(int argc, char **argv)
{
    int n = atoi(argv[1]);
	printf("%d*4: %u\n", n, (unsigned int)multiplyWith4(n));
    return 0;
}

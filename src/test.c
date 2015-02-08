#include <stdio.h>

int main() {
	int i = 10;
	int j = -1;
	int **pp;printf("hello");
	*pp = &i;
	printf("i: %u; j: %u; **pp: %d", (unsigned)i, (unsigned)j, **pp);
	if ((unsigned)i < (unsigned)j)
		printf("nice\n");
	return 0;
}

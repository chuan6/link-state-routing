#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs542.h"

graph *makegraph(int n) {
	if (n <= 0) {
		fprintf(stderr, "makegraph: non-positive argument is passed.\n");
		return NULL;
	}
	graph *g = (graph *)calloc(1, sizeof(graph));
	g->size = n;
	g->adjmtx = (int *)calloc(n * n, sizeof(int));
	return g;
}

void freegraph(graph **g) {
	if (*g == NULL)
		return;
	free((*g)->adjmtx);
	free(*g);
	*g = NULL;
}

void printgraph(graph *g) {
	if (g == NULL) {
		fprintf(stderr, "printgraph: NULL graph pointer is passed as argument.\n");
		return;
	}
	int n = g->size;
	//printf("%d routers\n", n);
	int i;
	int j;
	int *p = g->adjmtx;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%d\t", *(p + i*n + j));
		}
		printf("\n");
	}
}

int isvalidgraph(graph *g) {
	int n = g->size;
	int *p = g->adjmtx;
	int i, j;

	for (i = 0; i < n; i++) {
		for (j = i; j < n; j++) {
			if (i == j && *(p + i*n + j) != 0)
				return 0;	// upper-left to bottom-right diagonal is not all-zero
			if (*(p + i*n + j) != *(p + j*n + i))
				return 0;	// upper-right triangle and bottom-left triangle are not symmetric
		}
	}
	return 1;
}
#include <stdio.h>
#include <stdlib.h>
#include "cs542.h"

static int less(int a, int b) {
	return (unsigned)a < (unsigned)b;
}

static vertex *pickMIN(vertex **p, int len) {//printf("inside pickMIN\n");fflush(stdout);
	static vertex start = { 0, INFINITY, NULL };

	int i;
	vertex *r;
	vertex *sp = &start;
	vertex **min = &sp;
	for (i = 0; i < len; i++, p++) {
		if (*p == NULL)
			continue;
		if (less((*p)->dist, (*min)->dist))
			min = p;
	}
	if (*min == &start)	// all varr have dist==INFINITY
		return NULL;
	// min element is found
	r = *min;
	*min = NULL;
	return r;
}

// direct distance from vertex with id==a to vertex with id==b
static int distance(graph *g, int a, int b) {//printf("inside distance: a %d; b %d\n", a, b);fflush(stdout);
	--a;	// id to index; id starts with 1, index starts with 0
	--b;	// id to index
	return *(g->adjmtx + a * g->size + b);
}

void dijkstra(graph *g, int srcid, vertex *varr) {//printf("inside dijkstra %d\n", g->size);printf("world");
	int n = g->size; //printf("hello");
	vertex **s_set = (vertex **)calloc(n, sizeof(char *));
	vertex **q_set = (vertex **)calloc(n, sizeof(char *));
	int i;
	//printf("dijkstra: done set init\n");

	vertex **p;
	for (i = 1, p = q_set; i < n+1; i++, p++) {//printf("for-loop\n");
		*p = varr + i - 1;
		if (i == srcid) {
			(*p)->dist = 0;
		} else {
			(*p)->dist = INFINITY;
		}
		(*p)->pre = NULL;
	}
	//printf("dijkstra: done init\n");fflush(stdout);

	vertex *u;
	int s_set_i = 0;	
	int delta;
	int dist;
	vertex *vp;
	while ((u = pickMIN(q_set, n)) != NULL) {
		*(s_set + s_set_i++) = u;
		// relaxation
		for (i=1, vp=varr; i < n+1; i++, vp++) {
			if ((delta = distance(g, u->id, i)) == INFINITY) {
				//printf("distance returns INFINITY\n");
				fflush(stdout);
				continue;
			}
			//printf("distance returns %d\n", delta);fflush(stdout);

			dist = u->dist + delta;
			if (less(dist, vp->dist)) {
				vp->dist = dist;
				vp->pre = u;
			}
		}
	}
	free(s_set);
	free(q_set);
}
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "cs542.h"

#define SELF 0
#define NONE -1

router *
makerouter(int id, int n) {
	router *r = (router *)calloc(1, sizeof(router));
	r->id = id;
	r->n = n;
	r->varray = (vertex *)calloc(n, sizeof(vertex));
	int i;
	for (i = 0; i < n; i++) {
		(r->varray + i)->id = i + 1;
	}
	r->table = (entry *)calloc(n, sizeof(entry));
	return r;
}

void
freerouter(router *r) {
	if (r == NULL)
		return;
	free(r->varray);
	free(r->table);
	free(r);
}

void
printrouter(router *r) {
	int i;
        int n = r->n;
	for (i=0; i < n; i++) {
		printf("destination: %d\tnext-hop: %d\n",
                       r->table[i].dest, r->table[i].next);
	}
	fflush(stdout);
}

void printpath(router *src_router, int dest) {
        int src = src_router->id;
	if (dest == src) {
		printf("The destination router is the source router. No need to go through a path. The total cost is 0.\n");
		return;
	}

        vertex *dest_vtx = src_router->varray + dest - 1;
	if (dest_vtx->dist == INFINITY) {
		printf("The destination router is isolated. No path is found. The total cost is INFINITY.\n");
		return;
	}

	printf("The shortest path from %d to %d is %d", src, dest, src);

        int *sp = (int *) calloc(src_router->n - 1, sizeof(int));
        vertex *vp = dest_vtx;
        assert(dest = vp->id);
        int i = -1, id = dest;
        do {
                sp[++i] = id;
                vp = vp->pre;
                id = vp->id;
        } while (id != src);
        for (; i > -1; i--) {
                printf("-%d", sp[i]);
        }
	printf(", the total cost is %d.\n", dest_vtx->dist);
        free(sp);
}

static int nexthop(router *src, int dest) {
	if (dest == src->id)	// dest is the src router
		return SELF;
	if ((src->varray+dest-1)->dist == INFINITY)	// dest is an isolated vertex in the graph
		return NONE;
	vertex *p = src->varray + dest - 1;
	while (p->pre->id != src->id)
		p = p->pre;
	// p->pre == src
	return p->id;
}

void buildtable(router *r, graph *g) {
	dijkstra(g, r->id, r->varray);//printf("dijkstra ends\n");fflush(stdout);

	int i;
	entry *p;
	for (i=0, p=r->table; i < r->n; i++, p++) {//printf("buildtable for-loop %d\n", i); fflush(stdout);
		p->dest = i + 1;
		p->next = nexthop(r, p->dest);
	}
}

#define INFINITY -1

typedef struct vertex {
        int id;
        int dist;
        struct vertex *pre;
} vertex;

typedef struct graph {
        int size;    // number of vertices
        int *adjmtx; // store weight of all edges
} graph;

int isvalidgraph(graph *);
graph *makegraph(int);
void freegraph(graph **);
void printgraph(graph *);

typedef struct {
        int dest;
        int next;
} entry;

typedef struct router {
        int id;
        vertex *varray;
        int n; // number of routers on the network
        entry *table;
} router;

router *makerouter(int, int);
void freerouter(router *);
void printrouter(router *);
void buildtable(router *, graph *);
void printpath(router *, int);

void dijkstra(graph *, int, vertex *);

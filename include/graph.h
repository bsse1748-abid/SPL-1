#ifndef GRAPH_H
#define GRAPH_H

#define MAX_ROUTERS 20
#define NAME_LEN    16
#define NO_LINK     0          /* cost value meaning "no link" */
#define INF         1000000    /* "unreachable" distance (used from Day 2) */

#define STATUS_DOWN 0
#define STATUS_UP   1

typedef struct {
    int  id;
    char name[NAME_LEN];
    int  status;               /* STATUS_UP or STATUS_DOWN */
} Router;

typedef struct {
    Router routers[MAX_ROUTERS];
    int cost[MAX_ROUTERS][MAX_ROUTERS];     /* 0 = no link, else link cost */
    int link_up[MAX_ROUTERS][MAX_ROUTERS];  /* 1 = working, 0 = failed */
    int count;                              /* routers currently in network */
} Network;

/* ---- network management (graph.c) ---- */
void init_network(Network *net);
int  add_router(Network *net, const char *name);          /* id, or -1 on error */
int  find_router(const Network *net, const char *name);   /* id, or -1 if not found */
int  add_link(Network *net, const char *nameA, const char *nameB, int cost);
int  remove_link(Network *net, const char *nameA, const char *nameB);
void display_network(const Network *net);

/* a link can be used only if it exists, is UP, and both routers are UP */
int  edge_usable(const Network *net, int u, int v);

/* builds the 5-router example network used for testing and demos */
void load_sample_network(Network *net);

#endif

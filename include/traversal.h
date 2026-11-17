#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include "graph.h"

/*
 * Breadth-First Search from src.
 *   hops[v]   = number of links from src to v   (-1 if v was not reached)
 *   parent[v] = router that discovered v        (-1 for src / unreached)
 *   order[]   = routers in the order they were visited
 * All arrays need at least MAX_ROUTERS elements.
 * Returns how many routers were reached (0 if src is invalid or DOWN).
 */
int bfs(const Network *net, int src, int hops[], int parent[], int order[]);

/*
 * Depth-First Search from src (recursive).
 *   order[] = routers in the order they were visited
 * Returns how many routers were reached.
 */
int dfs(const Network *net, int src, int order[]);

/* 1 if every UP router can reach every other UP router, otherwise 0. */
int is_connected(const Network *net);

/* ---- menu helpers: look up names, run the search, print the result ---- */
void show_bfs(const Network *net, const char *from);
void show_dfs(const Network *net, const char *from);
void show_connectivity(const Network *net);

#endif

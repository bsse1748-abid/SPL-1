#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

/*
 * Run Dijkstra's algorithm from router src.
 *   dist[v] = cheapest cost from src to v   (INF if unreachable)
 *   prev[v] = router before v on that path  (-1 if none)
 * Both arrays must have at least MAX_ROUTERS elements.
 * Failed links and DOWN routers are skipped (see edge_usable in graph.c).
 */
void dijkstra(const Network *net, int src, int dist[], int prev[]);

/* First router after src on the best path to dest, or -1 if none. */
int next_hop(int src, int dest, const int dist[], const int prev[]);

/* Look up two routers by name, run Dijkstra, print path, cost and hops.
 * Returns 1 if a route exists, 0 otherwise. */
int show_shortest_path(const Network *net, const char *from, const char *to);

/* Print cost, next hop and path from one router to every other router. */
int show_all_paths(const Network *net, const char *from);

#endif

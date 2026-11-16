#include <stdio.h>
#include "dijkstra.h"

/* ---------- core algorithm ---------- */

void dijkstra(const Network *net, int src, int dist[], int prev[]) {
    int visited[MAX_ROUTERS] = {0};

    /* 1. initialise: everything unreachable, no previous router */
    for (int i = 0; i < net->count; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }

    if (src < 0 || src >= net->count || net->routers[src].status == STATUS_DOWN)
        return;                                   /* nothing is reachable */

    dist[src] = 0;

    for (int round = 0; round < net->count; round++) {
        /* 2. pick the closest router not yet finalised */
        int u = -1;
        for (int i = 0; i < net->count; i++) {
            if (!visited[i] && dist[i] < INF && (u == -1 || dist[i] < dist[u]))
                u = i;
        }
        if (u == -1)
            break;                                /* the rest are unreachable */
        visited[u] = 1;

        /* 3. relax: can we reach a neighbour more cheaply through u? */
        for (int v = 0; v < net->count; v++) {
            if (!visited[v] && edge_usable(net, u, v)) {
                int new_dist = dist[u] + net->cost[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    prev[v] = u;
                }
            }
        }
    }
}

int next_hop(int src, int dest, const int dist[], const int prev[]) {
    if (src == dest || dist[dest] >= INF)
        return -1;
    int v = dest;
    while (prev[v] != src)
        v = prev[v];                              /* walk back to the first step */
    return v;
}

/* ---------- printing helpers ---------- */

/* Print "R1 -> R3 -> R5" (no newline). Returns the number of hops. */
static int print_route(const Network *net, int dest, const int prev[]) {
    int path[MAX_ROUTERS];
    int len = 0;

    for (int v = dest; v != -1; v = prev[v])      /* collected backwards */
        path[len++] = v;

    for (int i = len - 1; i >= 0; i--) {
        printf("%s", net->routers[path[i]].name);
        if (i > 0)
            printf(" -> ");
    }
    return len - 1;
}

int show_shortest_path(const Network *net, const char *from, const char *to) {
    int src  = find_router(net, from);
    int dest = find_router(net, to);

    if (src == -1)  { printf("Error: router '%s' not found.\n", from); return 0; }
    if (dest == -1) { printf("Error: router '%s' not found.\n", to);   return 0; }

    int dist[MAX_ROUTERS], prev[MAX_ROUTERS];
    dijkstra(net, src, dist, prev);

    if (dist[dest] >= INF) {
        printf("No route from %s to %s.\n", from, to);
        return 0;
    }

    printf("Shortest path: ");
    int hops = print_route(net, dest, prev);
    printf("\nTotal cost: %d   Hops: %d\n", dist[dest], hops);

    int nh = next_hop(src, dest, dist, prev);
    if (nh != -1)
        printf("Next hop from %s: %s\n", from, net->routers[nh].name);
    return 1;
}

int show_all_paths(const Network *net, const char *from) {
    int src = find_router(net, from);
    if (src == -1) {
        printf("Error: router '%s' not found.\n", from);
        return 0;
    }

    int dist[MAX_ROUTERS], prev[MAX_ROUTERS];
    dijkstra(net, src, dist, prev);

    printf("\nBest paths from %s:\n", from);
    printf("%-8s %-8s %-10s %s\n", "To", "Cost", "Next hop", "Path");
    for (int d = 0; d < net->count; d++) {
        if (d == src)
            continue;
        if (dist[d] >= INF) {
            printf("%-8s %-8s %-10s %s\n", net->routers[d].name,
                   "-", "-", "unreachable");
            continue;
        }
        printf("%-8s %-8d %-10s ", net->routers[d].name, dist[d],
               net->routers[next_hop(src, d, dist, prev)].name);
        print_route(net, d, prev);
        printf("\n");
    }
    printf("\n");
    return 1;
}

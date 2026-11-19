#include <stdio.h>
#include "traversal.h"

/* ---------- BFS ---------- */

int bfs(const Network *net, int src, int hops[], int parent[], int order[]) {
    int queue[MAX_ROUTERS];
    int front = 0, rear = 0;
    int visited_count = 0;

    for (int i = 0; i < net->count; i++) {
        hops[i] = -1;                 /* -1 also means "not visited yet" */
        parent[i] = -1;
    }

    if (src < 0 || src >= net->count || net->routers[src].status == STATUS_DOWN)
        return 0;

    hops[src] = 0;
    queue[rear++] = src;

    while (front < rear) {
        int u = queue[front++];       /* take the oldest router in the queue */
        order[visited_count++] = u;

        for (int v = 0; v < net->count; v++) {
            if (hops[v] == -1 && edge_usable(net, u, v)) {
                hops[v] = hops[u] + 1;   /* mark as visited when it is queued */
                parent[v] = u;
                queue[rear++] = v;
            }
        }
    }
    return visited_count;
}

/* ---------- DFS ---------- */

static void dfs_visit(const Network *net, int u, int visited[],
                      int order[], int *count) {
    visited[u] = 1;
    order[(*count)++] = u;

    for (int v = 0; v < net->count; v++) {
        if (!visited[v] && edge_usable(net, u, v))
            dfs_visit(net, v, visited, order, count);   /* go deeper first */
    }
}

int dfs(const Network *net, int src, int order[]) {
    int visited[MAX_ROUTERS] = {0};
    int count = 0;

    if (src < 0 || src >= net->count || net->routers[src].status == STATUS_DOWN)
        return 0;

    dfs_visit(net, src, visited, order, &count);
    return count;
}

/* ---------- connectivity ---------- */

int is_connected(const Network *net) {
    int hops[MAX_ROUTERS], parent[MAX_ROUTERS], order[MAX_ROUTERS];
    int start = -1, up_count = 0;

    for (int i = 0; i < net->count; i++) {
        if (net->routers[i].status == STATUS_UP) {
            up_count++;
            if (start == -1)
                start = i;
        }
    }
    if (start == -1)
        return 0;                     /* no router is UP */

    return bfs(net, start, hops, parent, order) == up_count;
}

/* ---------- printing helpers for the menu ---------- */

void show_bfs(const Network *net, const char *from) {
    int src = find_router(net, from);
    if (src == -1) {
        printf("Error: router '%s' not found.\n", from);
        return;
    }

    int hops[MAX_ROUTERS], parent[MAX_ROUTERS], order[MAX_ROUTERS];
    int n = bfs(net, src, hops, parent, order);

    if (n == 0) {
        printf("Router %s is DOWN, nothing can be reached.\n", from);
        return;
    }

    printf("BFS from %s (visit order, hops from source):\n", from);
    for (int i = 0; i < n; i++) {
        int v = order[i];
        printf("  %-8s %d hop%s", net->routers[v].name, hops[v], hops[v] == 1 ? " " : "s");
        if (parent[v] != -1)
            printf("  (found from %s)", net->routers[parent[v]].name);
        printf("\n");
    }
    printf("Reached %d of %d routers.\n", n, net->count);
}

void show_dfs(const Network *net, const char *from) {
    int src = find_router(net, from);
    if (src == -1) {
        printf("Error: router '%s' not found.\n", from);
        return;
    }

    int order[MAX_ROUTERS];
    int n = dfs(net, src, order);

    if (n == 0) {
        printf("Router %s is DOWN, nothing can be reached.\n", from);
        return;
    }

    printf("DFS from %s: ", from);
    for (int i = 0; i < n; i++) {
        printf("%s", net->routers[order[i]].name);
        if (i < n - 1)
            printf(" -> ");
    }
    printf("\nReached %d of %d routers.\n", n, net->count);
}

void show_connectivity(const Network *net) {
    if (net->count == 0) {
        printf("The network is empty.\n");
        return;
    }

    int hops[MAX_ROUTERS], parent[MAX_ROUTERS], order[MAX_ROUTERS];
    int start = -1, up_count = 0;

    for (int i = 0; i < net->count; i++) {
        if (net->routers[i].status == STATUS_UP) {
            up_count++;
            if (start == -1)
                start = i;
        }
    }
    if (start == -1) {
        printf("All routers are DOWN.\n");
        return;
    }

    int reached = bfs(net, start, hops, parent, order);
    if (reached == up_count) {
        printf("Network is CONNECTED: all %d working routers can reach each other.\n",
               up_count);
        return;
    }

    printf("Network is NOT connected. From %s, these working routers are unreachable:\n",
           net->routers[start].name);
    for (int i = 0; i < net->count; i++) {
        if (net->routers[i].status == STATUS_UP && hops[i] == -1)
            printf("  %s\n", net->routers[i].name);
    }
}

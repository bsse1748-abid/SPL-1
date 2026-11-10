#include <stdio.h>
#include <string.h>
#include "graph.h"

/* Reset the network to empty: no routers, no links. */
void init_network(Network *net) {
    memset(net, 0, sizeof(Network));
    net->count = 0;
}

/* Add a router with the given name. Returns its id, or -1 on error. */
int add_router(Network *net, const char *name) {
    if (net->count >= MAX_ROUTERS) {
        printf("Error: network is full (max %d routers).\n", MAX_ROUTERS);
        return -1;
    }
    if (name == NULL || name[0] == '\0') {
        printf("Error: router name cannot be empty.\n");
        return -1;
    }
    if (find_router(net, name) != -1) {
        printf("Error: router '%s' already exists.\n", name);
        return -1;
    }

    int id = net->count;
    net->routers[id].id = id;
    strncpy(net->routers[id].name, name, NAME_LEN - 1);
    net->routers[id].name[NAME_LEN - 1] = '\0';
    net->routers[id].status = STATUS_UP;

    /* the new router starts with no links */
    for (int i = 0; i <= id; i++) {
        net->cost[id][i] = NO_LINK;
        net->cost[i][id] = NO_LINK;
        net->link_up[id][i] = 0;
        net->link_up[i][id] = 0;
    }

    net->count++;
    return id;
}

/* Find a router id by name. Returns -1 if not found. */
int find_router(const Network *net, const char *name) {
    for (int i = 0; i < net->count; i++) {
        if (strcmp(net->routers[i].name, name) == 0)
            return i;
    }
    return -1;
}

/* Add (or update) a two-way link. Returns 1 on success, 0 on error. */
int add_link(Network *net, const char *nameA, const char *nameB, int cost) {
    int a = find_router(net, nameA);
    int b = find_router(net, nameB);

    if (a == -1) { printf("Error: router '%s' not found.\n", nameA); return 0; }
    if (b == -1) { printf("Error: router '%s' not found.\n", nameB); return 0; }
    if (a == b)  { printf("Error: cannot link a router to itself.\n"); return 0; }
    if (cost <= 0) { printf("Error: cost must be a positive number.\n"); return 0; }

    if (net->cost[a][b] != NO_LINK)
        printf("Note: link already exists (cost %d). Updating cost.\n", net->cost[a][b]);

    net->cost[a][b] = cost;
    net->cost[b][a] = cost;          /* links work in both directions */
    net->link_up[a][b] = 1;
    net->link_up[b][a] = 1;
    return 1;
}

/* Permanently remove a link. Returns 1 on success, 0 on error. */
int remove_link(Network *net, const char *nameA, const char *nameB) {
    int a = find_router(net, nameA);
    int b = find_router(net, nameB);

    if (a == -1) { printf("Error: router '%s' not found.\n", nameA); return 0; }
    if (b == -1) { printf("Error: router '%s' not found.\n", nameB); return 0; }
    if (net->cost[a][b] == NO_LINK) {
        printf("Error: no link between %s and %s.\n", nameA, nameB);
        return 0;
    }

    net->cost[a][b] = net->cost[b][a] = NO_LINK;
    net->link_up[a][b] = net->link_up[b][a] = 0;
    return 1;
}

/* A link is usable only if it exists, is UP, and both routers are UP. */
int edge_usable(const Network *net, int u, int v) {
    if (u < 0 || v < 0 || u >= net->count || v >= net->count)
        return 0;
    return net->cost[u][v] != NO_LINK
        && net->link_up[u][v] == 1
        && net->routers[u].status == STATUS_UP
        && net->routers[v].status == STATUS_UP;
}

/* Print routers, links, and the cost matrix. */
void display_network(const Network *net) {
    if (net->count == 0) {
        printf("The network is empty.\n");
        return;
    }

    printf("\n=== Routers (%d) ===\n", net->count);
    for (int i = 0; i < net->count; i++) {
        printf("  [%d] %-*s  %s\n", i, NAME_LEN - 1, net->routers[i].name,
               net->routers[i].status == STATUS_UP ? "UP" : "DOWN");
    }

    printf("\n=== Links ===\n");
    int links = 0;
    for (int i = 0; i < net->count; i++) {
        for (int j = i + 1; j < net->count; j++) {      /* j > i: print each link once */
            if (net->cost[i][j] != NO_LINK) {
                printf("  %s <-> %s   cost %d   %s\n",
                       net->routers[i].name, net->routers[j].name,
                       net->cost[i][j],
                       net->link_up[i][j] ? "UP" : "DOWN");
                links++;
            }
        }
    }
    if (links == 0)
        printf("  (no links yet)\n");

    printf("\n=== Cost matrix (0 = no link) ===\n");
    printf("%-8s", "");
    for (int j = 0; j < net->count; j++)
        printf("%-8s", net->routers[j].name);
    printf("\n");
    for (int i = 0; i < net->count; i++) {
        printf("%-8s", net->routers[i].name);
        for (int j = 0; j < net->count; j++)
            printf("%-8d", net->cost[i][j]);
        printf("\n");
    }
    printf("\n");
}

void load_sample_network(Network *net) {
    init_network(net);

    add_router(net, "R1");
    add_router(net, "R2");
    add_router(net, "R3");
    add_router(net, "R4");
    add_router(net, "R5");

    add_link(net, "R1", "R2", 2);
    add_link(net, "R2", "R3", 3);
    add_link(net, "R1", "R3", 4);
    add_link(net, "R1", "R4", 6);
    add_link(net, "R3", "R5", 1);
    add_link(net, "R4", "R5", 2);
}

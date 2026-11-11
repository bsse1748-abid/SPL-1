#include <stdio.h>
#include "graph.h"
#include "utils.h"

static void print_menu(void) {
    printf("\n===== Network Routing Simulator =====\n");
    printf(" 1. Add router\n");
    printf(" 2. Add link\n");
    printf(" 3. Remove link\n");
    printf(" 4. Display network\n");
    printf(" 5. Load sample network (R1-R5)\n");
    printf(" 0. Exit\n");
    printf("=====================================\n");
}

int main(void) {
    Network net;
    init_network(&net);

    int choice;
    do {
        print_menu();
        choice = read_int("Choose an option: ", 0, 5);

        switch (choice) {
        case 1: {
            char name[NAME_LEN];
            read_string("Router name: ", name, NAME_LEN);
            if (add_router(&net, name) != -1)
                printf("Router %s added.\n", name);
            break;
        }
        case 2: {
            char a[NAME_LEN], b[NAME_LEN];
            read_string("First router: ", a, NAME_LEN);
            read_string("Second router: ", b, NAME_LEN);
            int cost = read_int("Link cost (1-1000): ", 1, 1000);
            if (add_link(&net, a, b, cost))
                printf("Link %s <-> %s (cost %d) added.\n", a, b, cost);
            break;
        }
        case 3: {
            char a[NAME_LEN], b[NAME_LEN];
            read_string("First router: ", a, NAME_LEN);
            read_string("Second router: ", b, NAME_LEN);
            if (remove_link(&net, a, b))
                printf("Link %s <-> %s removed.\n", a, b);
            break;
        }
        case 4:
            display_network(&net);
            break;
        case 5:
            load_sample_network(&net);
            printf("Sample network loaded.\n");
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        }
    } while (choice != 0);

    return 0;
}

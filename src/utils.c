#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int read_int(const char *prompt, int min, int max) {
    char line[64];
    char *end;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput closed. Exiting.\n");
            exit(0);
        }

        long value = strtol(line, &end, 10);

        /* skip trailing spaces/newline, then we must be at the end of the text */
        while (*end == ' ' || *end == '\t' || *end == '\n')
            end++;

        if (end == line || *end != '\0') {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

void read_string(const char *prompt, char *buf, int size) {
    char line[128];

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nInput closed. Exiting.\n");
            exit(0);
        }

        /* remove the newline at the end */
        line[strcspn(line, "\n")] = '\0';

        /* trim leading spaces */
        char *start = line;
        while (*start && isspace((unsigned char)*start))
            start++;

        /* trim trailing spaces */
        int len = (int)strlen(start);
        while (len > 0 && isspace((unsigned char)start[len - 1]))
            start[--len] = '\0';

        if (len == 0) {
            printf("Input cannot be empty.\n");
            continue;
        }

        strncpy(buf, start, size - 1);
        buf[size - 1] = '\0';
        return;
    }
}

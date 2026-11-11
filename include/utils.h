#ifndef UTILS_H
#define UTILS_H

void clear_input_buffer(void);

int read_int(const char *prompt, int min, int max);

void read_string(const char *prompt, char *buf, int size);

#endif

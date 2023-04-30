#include <stdio.h>
#include <stdbool.h>

void print_int(int n) {
    printf("%d", n);
}

void print_bool(bool b) {
    if (b) {
        printf("true");
    } else {
        printf("false");
    }
}

void print_char(char c) {
    printf("%c", c);
}

void print_float(float f) {
    printf("%f", f);
}

void print_string(char* s) {
    printf("%s", s);
}

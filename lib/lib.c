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

int read_int() {
    int n;
    scanf("%d", &n);
    return n;
}

bool read_bool() {
    char buffer[5];
    scanf("%s", buffer);
    if (buffer[0] == 't' || buffer[0] == 'T') {
        return true;
    } else {
        return false;
    }
}

char read_char() {
    char c;
    scanf(" %c", &c);
    return c;
}

float read_float() {
    float f;
    scanf("%f", &f);
    return f;
}

void read_string(char* s) {
    scanf("%s", s);
}

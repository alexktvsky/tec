#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

struct foo_s_int {
    int data;
    int counter;
};

struct foo_s_double {
    double data;
    int counter;
};

int main(void) {

    struct foo_s_int f1;
    struct foo_s_double f2;

    return 0;
}


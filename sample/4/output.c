#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct foo_s_int {
    int data;
    int counter;
} foo_t_int;

typedef struct foo_s_double {
    double data;
    int counter;
} foo_t_double;

typedef struct foo_s_void_ptr {
    void *data;
    int counter;
} foo_t_void_ptr;


int main(void) {

    foo_t_int f1;
    foo_t_double f2;

    struct foo_s_void_ptr f3;

    return 0;
}

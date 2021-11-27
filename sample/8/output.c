#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    int data;
} node_t_int;

typedef struct {
    double data;
} node_t_double;

typedef struct {
    node_t_int *head;
    node_t_int *tail;
} list_t_int;

typedef struct {
    node_t_double *head;
    node_t_double *tail;
} list_t_double;

void foo_int(int a) {
    list_t_int l1;
}

void foo_double(double a) {
    list_t_double l1;
}


int main(void) {

    foo_int(10);
    foo_double(10.1);

    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    int value;
} foo_t_int;

typedef struct {
    double value;
} foo_t_double;

typedef struct {
    foo_t_int data;
} node_t_foo_t_int;

typedef struct {
    foo_t_double data;
} node_t_foo_t_double;

typedef struct {
    node_t_foo_t_int *head;
    node_t_foo_t_int *tail;
} list_t_int;

typedef struct {
    node_t_foo_t_double *head;
    node_t_foo_t_double *tail;
} list_t_double;


int main(void) {

    list_t_int l1;
    list_t_double l2;

    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    double value;
} foo_t_double;

typedef struct {
    char *value;
} foo_t_charptr;

typedef struct {
    int x1;
    foo_t_double x2;
} node_t_int_foo_t_double;

typedef struct {
    void *x1;
    foo_t_charptr x2;
} node_t_voidptr_foo_t_charptr;

typedef struct {
    node_t_int_foo_t_double *head;
    node_t_int_foo_t_double *tail;
} list_t_int_double;

typedef struct {
    node_t_voidptr_foo_t_charptr *head;
    node_t_voidptr_foo_t_charptr *tail;
} list_t_voidptr_charptr;


int main(void) {

    list_t_int_double l1;
    list_t_voidptr_charptr l2;

    return 0;
}

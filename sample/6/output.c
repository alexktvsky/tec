#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    int first;
    unsigned long second;
} pair_t_int_unsignedlong;

typedef struct {
    char *first;
    size_t second;
} pair_t_charptr_size_t;

typedef struct {
    pair_t_int_unsignedlong data;
} node_t_pair_t_int_unsignedlong;

typedef struct {
    pair_t_charptr_size_t data;
} node_t_pair_t_charptr_size_t;

typedef struct {
    node_t_pair_t_int_unsignedlong *head;
    node_t_pair_t_int_unsignedlong *tail;
} list_t_int_unsignedlong;

typedef struct {
    node_t_pair_t_charptr_size_t *head;
    node_t_pair_t_charptr_size_t *tail;
} list_t_charptr_size_t;

int main(void) {

    list_t_int_unsignedlong l1;
    list_t_charptr_size_t l2;

    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct node_s_int node_t_int;
typedef struct node_s_double node_t_double;

struct node_s_int {
    int data;
    node_t_int *next;
    node_t_int *prev;
};

struct node_s_double {
    double data;
    node_t_double *next;
    node_t_double *prev;
};

typedef struct {
    node_t_int *head;
    node_t_int *tail;
} list_t_int;

typedef struct {
    node_t_double *head;
    node_t_double *tail;
} list_t_double;


int main(void) {

    list_t_int l1;
    list_t_double l2;

    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    int data;
    int counter;
} foo_t_int;

typedef struct {
    double data;
    int counter;
} foo_t_double;


int main(void) {

    foo_t_int f1;
    foo_t_double f2;

    return 0;
}

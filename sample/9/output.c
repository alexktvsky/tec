#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct data_s_int data_t_int;

typedef struct data_s_double data_t_double;

struct data_s_int {
    int value;
};

struct data_s_double {
    double value;
};


int main(void) {

    data_t_int l1;
    data_t_double l2;

    return 0;
}

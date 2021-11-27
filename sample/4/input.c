#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct foo_s {
    T data;
    int counter;
} foo_t;


int main(void) {

    foo_t<int> f1;
    foo_t<double> f2;

    struct foo_s<void *> f3;

    return 0;
}

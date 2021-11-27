#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct {
    T data;
    int counter;
} foo_t;


int main(void) {

    foo_t<int> f1;
    foo_t<double> f2;

    return 0;
}

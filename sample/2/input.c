#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
struct foo_s {
    T data;
    int counter;
};


int main(void) {

    struct foo_s<int> f1;
    struct foo_s<double> f2;

    return 0;
}

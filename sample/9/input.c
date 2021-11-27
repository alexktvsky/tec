#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct data_s<T> data_t<T>;

template <typename T>
struct data_s {
    T value;
};


int main(void) {

    data_t<int> l1;
    data_t<double> l2;

    return 0;
}

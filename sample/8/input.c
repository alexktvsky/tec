#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct {
    T data;
} node_t;

template <typename T>
typedef struct {
    node_t<T> *head;
    node_t<T> *tail;
} list_t;


template <typename T>
void foo(T a) {
    list_t<T> l1;
}

int main(void) {

    foo<int>(10);
    foo<double>(10.1);

    return 0;
}

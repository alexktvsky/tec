#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct {
    T value;
} foo_t;

template <typename T>
typedef struct {
    T data;
} node_t;

template <typename T>
typedef struct {
    node_t<foo_t<T>> *head;
    node_t<foo_t<T>> *tail;
} list_t;


int main(void) {

    list_t<int> l1;
    list_t<double> l2;

    return 0;
}

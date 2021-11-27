#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct {
    T value;
} foo_t;

template <typename T1, typename T2>
typedef struct {
    T1 x1;
    T2 x2;
} node_t;

template <typename T1, typename T2>
typedef struct {
    node_t<T1, foo_t<T2>> *head;
    node_t<T1, foo_t<T2>> *tail;
} list_t;


int main(void) {

    list_t<int, double> l1;
    list_t<void *, char *> l2;

    return 0;
}

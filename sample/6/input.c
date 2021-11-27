#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T1, typename T2>
typedef struct {
    T1 first;
    T2 second;
} pair_t;

template <typename T>
typedef struct {
    T data;
} node_t;

template <typename T1, typename T2>
typedef struct {
    node_t<pair_t<T1, T2>> *head;
    node_t<pair_t<T1, T2>> *tail;
} list_t;


int main(void) {

    list_t<int, unsigned long> l1;
    list_t<char *, size_t> l2;

    return 0;
}

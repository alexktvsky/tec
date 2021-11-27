#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
typedef struct node_s<T> node_t<T>;

template <typename T>
struct node_s {
    T data;
    node_t<T> *next;
    node_t<T> *prev;
};

template <typename T>
typedef struct {
    node_t<T> *head;
    node_t<T> *tail;
} list_t;


int main(void) {

    list_t<int> l1;
    list_t<double> l2;

    return 0;
}

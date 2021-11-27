#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


template <typename T>
int is_equal(T a, T b);

template <typename T>
int is_equal(T a, T b) {
    return ((a == b) ? 1 : 0);
}


template <typename T1, typename T2>
int foo(T1 a, T2 b) {
    return ((a == b) ? 1 : 0);
}


int main(void) {

    printf("%d\n", is_equal<int>(0, 0));
    printf("%d\n", is_equal<void *>(NULL, NULL));
    printf("%d\n", foo<void *, void *>(NULL, NULL));

    return 0;
}
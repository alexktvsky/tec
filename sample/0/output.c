#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


int is_equal_int(int a, int b) {
    return ((a == b) ? 1 : 0);
}

int is_equal_void_ptr(void *a, void *b) {
    return ((a == b) ? 1 : 0);
}


int main(void) {

    printf("%d\n", is_equal_int(0, 0));
    printf("%d\n", is_equal_void_ptr(NULL, NULL));

    return 0;
}
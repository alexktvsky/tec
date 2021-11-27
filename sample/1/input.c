#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

template <typename T>
T max(T a, T b);

template <typename T>
T max(T a, T b) {
    return ((a > b) ? a : b);
}


int main(void) {

    printf("%d\n", max<size_t>(11, 22));
    printf("%d\n", max<size_t>(128, 256));
    printf("%f\n", max<double>(11.1, 22.2));
    printf("%f\n", max<unsigned long>(256, 512));

    return 0;
}

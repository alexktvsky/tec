#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

size_t max_size_t(size_t a, size_t b);

double max_double(double a, double b);

unsigned long max_unsignedlong(unsigned long a, unsigned long b);

size_t max_size_t(size_t a, size_t b) {
    printf("max\n");
    return ((a > b) ? a : b);
}

double max_double(double a, double b) {
    printf("max\n");
    return ((a > b) ? a : b);
}

unsigned long max_unsignedlong(unsigned long a, unsigned long b) {
    printf("max\n");
    return ((a > b) ? a : b);
}


int main(void) {

    printf("%d\n", max_size_t(11, 22));
    printf("%d\n", max_size_t(128, 256));
    printf("%f\n", max_double(11.1, 22.2));
    printf("%f\n", max_unsignedlong(256, 512));

    return 0;
}

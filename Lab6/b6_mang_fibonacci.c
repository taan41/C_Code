#include <stdio.h>

int main() {
    int fib[20];
    fib[0] = 0;
    fib[1] = 1;

    for(int i = 2; i < 20; i++) fib[i] = fib[i - 1] + fib[i - 2];

    printf("20 so day tien cua day Fibonacci:\n");
    for(int i = 0; i < 20; printf("%d ", fib[i++]));

    return 0;
}
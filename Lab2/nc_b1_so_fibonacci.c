#include <stdio.h>

long long int fibonacci_recursive(int n);
long long int fibonacci(int n);

int main() {
    int recursion_bool = 0, n;
    long long int result;

    while (1) {
        printf("Nhap 1 de dung de quy, 0 de dung phep lap, -1 de ket thuc: ");
        scanf("%d", &recursion_bool);
        
        if (recursion_bool == -1) {
            printf("\nKet thuc...\n");
            break;
        }

        printf("Nhap n: ");
        scanf("%d", &n);

        result = recursion_bool ? fibonacci_recursive(n) : fibonacci(n);
        printf("So fibonacci tai vi tri thu %d la: %lld\n\n", n, result);
    }

    return 0;
}

long long int fibonacci_recursive(int n) {
    return n < 2 ? n : fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

long long int fibonacci(int n) {
    if (n < 2) {
        return n;
    }

    long long int a = 0, b = 1, fib;
    for (int i = 2; i <= n; i++) {
        fib = a + b;
        a = b;
        b = fib;
    }

    return fib;
}
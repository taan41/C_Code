#include <stdio.h>

int main() {
    int n;

    printf("Nhap n: ");
    scanf("%d", &n);

    while(n < 1) {
        printf("Nhap so lon hon 0: ");
        scanf("%d", &n);
    }

    printf("%d so dau tien trong day Fibonacci:\n0 ", n);
    if(n == 1) return 0;

    long long int fib = 1, a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        printf("%lld ", fib);

        fib = a + b;
        a = b;
        b = fib;
    }

    return 0;
}
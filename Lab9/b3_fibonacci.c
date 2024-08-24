#include <stdio.h>

long long int fibonacci(int n) {
    if(n <= 1) return 0;
    if(n == 2) return 1;

    long long int fib = 0, a = 0, b = 1;
    for(int i = 3; i <= n; i++) {
        fib = a + b;
        a = b;
        b = fib;
    }

    return fib;
}

int main() {
    int n;
    
    printf("Nhap n: ");
    scanf("%d", &n);

    printf("So o vi tri thu %d trong day fibonacci la:\n%lld\n", n, fibonacci(n));
}
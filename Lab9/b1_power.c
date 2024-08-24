#include <stdio.h>

float power(int a, int n) {
    if(n < 0) return 1 / power(a, -n);

    int result = 1;
    while(n-- > 0) result *= a;
    return result;
}

int main() {
    int a, n;

    printf("Nhap a, n: ");
    scanf("%d%d", &a, &n);

    printf("%d^%d = %.2f", a, n, power(a, n));

    return 0;
}
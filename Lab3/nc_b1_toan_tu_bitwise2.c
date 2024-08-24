#include <stdio.h>

int main() {
    int a, b;

    printf("Nhap a & b: ");
    scanf("%d%d", &a, &b);

    int and = a & b;
    int or = a | b;
    int xor = a ^ b;
    int not = ~a;

    printf("and: a & b = %d\n", and);
    printf("or: a | b = %d\n", or);
    printf("or: a ^ b = %d\n", xor);
    printf("or: ~a = %d\n", not);

    return 0;
}
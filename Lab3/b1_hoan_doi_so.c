#include <stdio.h>

int main() {
    int a, b;
    printf("Nhap a & b: ");
    scanf("%d%d", &a, &b);

    int c = a;
    a = b;
    b = c;

    a++;
    b++;

    printf("Gia tri cua a & b sau khi hoan doi va tang len 1: %d, %d", a, b);

    return 0;
}
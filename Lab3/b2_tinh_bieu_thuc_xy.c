#include <stdio.h>

int main() {
    int x, y, ex;

    printf("Nhap x & y: ");
    scanf("%d", &x, &y);

    ex = x*x*x + 3*x*x + 3*x*y*y + y*y*y;

    printf("Ket qua cua bieu thuc x^3 + 3x^2 + 3xy^2 + y^3 = %d", ex);

    return 0;
}
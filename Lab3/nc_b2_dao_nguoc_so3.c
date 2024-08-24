#include <stdio.h>

int main() {
    int a;

    printf("Nhap 1 so co 6 chu so: ");
    scanf("%d", &a);

    while (a < 100000 || a > 999999) {
        printf("So khong hop le, xin nhap lai: ");
        scanf("%d, &a");
    }

    int result = 0;
    while (a != 0) {
        result = result * 10 + a % 10;
        a /= 10;
    }

    printf("So dao nguoc: %d\n", result);
}
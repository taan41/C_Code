#include <stdio.h>

int main() {
    int a, b, x;

    printf("Nhap a, b: ");
    scanf("%d%d", &a, &b);

    printf("%s", a == 0 ? (b == 0 ? "Phuong trinh co vo so nghiem.\n" : "Phuong trinh vo nghiem.\n") : "Nghiem: ");
    printf(a == 0 ? "" : "%d\n", -b/a);

    return 0;
}
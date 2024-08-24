#include <stdio.h>

int main() {
    int a, b, c;

    printf("Nhap 3 so a, b, c: ");
    scanf("%d%d%d", &a, &b, &c);

    if (a + b > c && a + c > b && b + c > a) {
        printf("a, b, c la 3 canh tam giac.\n");
    } else {
        printf("a, b, c khong phai 3 canh tam giac.\n");
    }

    return 0;
}
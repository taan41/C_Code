#include <stdio.h>

int main() {
    int a, b, c;

    printf("Nhap a, b, c: ");
    scanf("%d%d%d", &a, &b, &c);

    // if (a >= b && a >= c) printf("MAX = %d\n", a);
    // else if (b >= c) printf("MAX = %d\n", b);
    // else printf("MAX = %d\n", c);

    if (a >= b && a >= c) printf("MAX = %d\n", a);
    else if (b >= a && b >= c) printf("MAX = %d\n", b);
    else if (c >= a && c >= b) printf("MAX = %d\n", c);

    return 0;
}
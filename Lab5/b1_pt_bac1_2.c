#include <stdio.h>

int main() {
    double a, b;

    printf(
        "Phuong trinh co dang ax + b = 0\n"
        "Nhap a, b: "
    );
    scanf("%lf%lf", &a, &b);

    if (a == 0) {
        if (b == 0) printf("Phuong trinh co vo so nghiem.\n");
        else printf("Phuong trinh vo nghiem.\n");
    } else {
        printf("Phuong trinh co nghiem: %.2lf\n", -b/a);
    }

    return 0;
}
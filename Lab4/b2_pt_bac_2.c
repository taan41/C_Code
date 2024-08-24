#include <stdio.h>
#include <math.h>

int main() {
    double a, b, c;
    double delta, x1, x2;

    printf("Phuong trinh co dang ax^2 + bx + c = 0\n");
    printf("Nhap a, b, c: ");
    scanf("%lf%lf%lf", &a, &b, &c);

    if (a == 0) {
        if (b == 0) {
            if (c == 0) printf("Phuong trinh co vo so nghiem.\n");
            else printf("Phuong trinh vo nghiem.\n");
        } else {
            x1 = -c / b;
            printf("Phuong trinh co 1 nghiem: %.2lf\n", x1);
        }
    } else {
        delta = b * b - 4 * a * c;
        
        if (delta > 0) {
            x1 = (-b + sqrt(delta)) / (2 * a);
            x2 = (-b - sqrt(delta)) / (2 * a);
            printf("Phuong trinh co 2 nghiem:\n%.2lf\n%.2lf\n", x1, x2);
        } else if (delta == 0) {
            x1 = -b / (2 * a);
            printf("Phuong trinh co 1 nghiem: %.2lf\n", x1);
        } else {
            printf("Phuong trinh vo nghiem.\n");
        }
    }

    return 0;
}
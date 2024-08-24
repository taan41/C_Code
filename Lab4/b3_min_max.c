#include <stdio.h>

int main() {
    double a, b, c, d;
    double min, max;

    printf("Nhap a, b, c, d: ");
    scanf("%lf%lf%lf%lf", &a, &b, &c, &d);

    min = a;
    max = a;

    if (min > b) min = b;
    if (max < b) max = b;

    if (min > c) min = c;
    if (max < c) max = c;

    if (min > d) min = d;
    if (max < d) max = d;

    printf("So lon nhat la: %.2lf\n", max);
    printf("So nho nhat la: %.2lf\n", min);

    return 0;
}
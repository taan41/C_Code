#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>


int main() {
    double R, h, SA, V;

    printf("Nhap ban kinh day: ");
    scanf("%lf", &R);
    printf("Nhap chieu cao tru: ");
    scanf("%lf", &h);

    SA = M_PI * R * R;
    V = SA * h;

    printf("Dien tich day tru: %.2lf\nThe tich tru: %.2lf\n", SA, V);
}
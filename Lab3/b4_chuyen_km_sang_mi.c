#include <stdio.h>

int main() {
    double conversion = 0.000621371192, km;

    printf("1 meter = %.12lf mile\n", conversion);
    printf("---------------------\n");
    printf("Moi ban nhap so km: ");
    scanf("%lf", &km);
    printf("Ket qua:\n");
    printf("%.2lf km = %.6lf mile", km, 1000 * km * conversion);

    return 0;
}
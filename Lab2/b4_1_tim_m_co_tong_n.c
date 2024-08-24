#include <stdio.h>
#include <math.h>

int main() {
    double n, a;
    int m;
    scanf("%lf", &n);
    a = (-1 + sqrt(1 + 8 * n)) / 2;
    m = (int) a;
    printf("%.2lf\n%d\n", a, m);

    return 0;
}
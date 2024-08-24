#include <stdio.h>

long long int factorial(int n);
long long int double_factorial(int n);
long long int superfactorial(int n);

int main() {
    int fact_type, n;

    printf("Nhap 1, 2, 3 cho giai thua, giai thua kep hoac sieu giai thua: ");
    scanf("%d", &fact_type);
    printf("Nhap n: ");
    scanf("%d", &n);
    //printf("%lld", double_factorial(n));

    for (int i = 1; i <= n; i++) {
        switch (fact_type) {
            case 1:
                printf("%d! = %lld\n", i, factorial(i));
                break;

            case 2:
                printf("%d!! = %lld\n", i, double_factorial(i));
                break;

            case 3:
                printf("sf(%d) = %lld\n", i, superfactorial(i));
                break;
            
            default:
                break;
        }
    }

    return 0;
}

long long int factorial(int n) {
    long long int result = 1;
    for (int i = n; i > 0; i--) {
        result *= i;
    }
    return result;
}

long long int double_factorial(int n)
{
    long long int result = 1;
    for (int i = n; i > 0; i -= 2) {
        result *= i;
    }
    return result;
}

long long int superfactorial(int n) {
    long long int result = 1;
    for (int i = n; i > 0; i--) {
        result *= factorial(i);
    }
    return result;
}

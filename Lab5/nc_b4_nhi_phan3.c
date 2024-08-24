#include <stdio.h>
#include <math.h>

int main() {
    long long int input;

    printf("Nhap mot so >= 0: ");
    scanf("%lld", &input);

    long long int result = 0;
    for(int i = 0; input > 0; i++) {
        result += (input % 2) * round(pow(10, i));
        input /= 2;
    }

    printf("%lld", result);

    return 0;
}
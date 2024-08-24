#include <stdio.h>
#include <stdlib.h>

int main() {
    int input, sum = 0;

    printf("Nhap so: ");
    scanf("%d", &input);

    while(input != 0) {
        sum += input % 10;
        input /= 10;
    }

    printf("Tong cac chu so: %d\n", sum);

    return 0;
}
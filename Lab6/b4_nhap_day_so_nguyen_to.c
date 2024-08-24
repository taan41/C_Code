#include <stdio.h>

int isPrime(int n);

int main() {
    int arr[5], input;

    for(int i = 0; i < 5; i++) {
        printf("Nhap so thu %d: ", i+1);
        scanf("%d", &input);

        while(!isPrime(input)) {
            printf("So vua nhap khong phai so nguyen to, hay nhap lai: ");
            scanf("%d", &input);
        }

        arr[i] = input;
    }

    printf("Day so vua nhap:\n");
    for(int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}

int isPrime(int n) {
    if(n <= 1) return 0;
    if(n <= 3) return 1;

    if(n % 2 == 0 || n % 3 == 0) return 0;

    for(int i = 5; i * i <= n; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }

    return 1;
}
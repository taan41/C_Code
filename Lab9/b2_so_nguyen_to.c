#include <stdio.h>


int inputNumber() {
    int n;
    scanf("%d", &n);
    return n;
}

int checkNumber(int num) {
    if(num < 2) return 0;
    if(num == 2 || num == 3) return 1;
    if(num % 2 == 0) return 0;

    for(int i = 3; i * i <= num; i += 2) if(num % i == 0) return 0;

    return 1;
}

int main() {
    int arr[7];

    printf("Nhap mang a co 7 phan tu:\n");
    for(int i = 0; i < 7; i++) {
        printf("Nhap phan tu thu %d: ", i + 1);
        arr[i] = inputNumber();
    }

    printf("Cac so nguyen to trong mang:\n");
    for(int i = 0; i < 7; i++) {
        if(checkNumber(arr[i])) printf("%d ", arr[i]);
    }
    return 0;
}
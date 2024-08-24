#include <stdio.h>

int main() {
    int arr[5], i = 0, *ptr = arr;

    printf("Nhap du lieu mang: ");
    while(i < 5) scanf("%d", ptr + i++);

    printf("Du lieu mang:\n");
    i = 0;
    while(i < 5) printf("%d ", *(arr + i++));

    return 0;
}
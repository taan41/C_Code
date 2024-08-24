#include <stdio.h>

int main() {
    int arr[10];
    for(int i = 0; i < 10; i++) {
        printf("Nhap phan tu thu %d: ", i);
        scanf("%d", &arr[i]);
    }

    for (int i = 0; i < 10; i+=2) {
        printf("So o vi tri thu %d la: %d\n", i, arr[i]);
    }

    return 0;
}
#include <stdio.h>

int main() {
    int n = 0;
    for(; n < 1;) {
        printf("Nhap kich co mang (>0): ");
        scanf("%d", &n);
    }

    int arr[n];
    printf("Nhap phan tu mang: ");
    for(int i = 0; i < n; scanf("%d", &arr[i++]));

    int swap, isSorted;
    for(int i = 0; i < n - 1; i++) {
        isSorted = 1;
        
        for(int j = 0; j < n - 1 - i; j++) {
            if(arr[j] > arr[j + 1]) {
                swap = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;

                isSorted = 0;
            }
        }

        if(isSorted) break;
    }

    printf("Mang sau khi sap xep:\n");
    for(int i = 0; i < n; printf("%d ", arr[i++]));

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n = 100;
    int arr[n];

    srand(time(NULL));

    printf("Mang chua sap xep:");
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 1001;

        if(i % 10 == 0) printf("\n");
        printf("%-5d ", arr[i]);
    }

    int swap;
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - 1 - i; j++) {
            if(arr[j] > arr[j + 1]) {
                swap = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = swap;
            }
        }
    }

    printf("\n\nMang sau sap xep:");
    for(int i = 0; i < n; i++) {
        if(i % 10 == 0) printf("\n");
        printf("%-5d ", arr[i]);
    }
    printf("\n");

    int input = -1;
    while(input < 0 || input > 1000) {
        printf("Nhap so tu 0 den 1000: ");
        scanf("%d", &input);
    }

    int pos = -1;
    for(int l = 0, r = n - 1; l <= r;) {
        int mid = (l + r) / 2;

        if(arr[mid] == input) {
            pos = mid;
            r = mid - 1;
        } else if (arr[mid] > input) r = mid - 1;
        else l = mid + 1;
    }

    if(pos == -1) printf("%d khong xuat hien trong mang\n", input);
    else printf("%d xuat hien trong mang tai vi tri %d\n", input, pos);

    return 0;
}
#include <stdio.h>

int main() {
    int arr[10] = {0};

    printf("Nhap 10 so: ");
    for(int i = 0; i < 10; i++) {
        scanf("%d", &arr[i]);
    }

    int min = arr[0], max = arr[0], min_count = 1, max_count = 1;
    for(int i = 1; i < 10; i++) {
        if(arr[i] == min) min_count++;
        else if(arr[i] < min) {
            min = arr[i];
            min_count = 1;
        }

        if(arr[i] == max) max_count++;
        else if (arr[i] > max) {
            max = arr[i];
            max_count = 1;
        }
    }

    printf(
        "Gia tri lon nhat la %d voi %d lan xuat hien.\n"
        "Gia tri be nhat la %d voi %d lan xuat hien.\n",
        max, max_count, min, min_count
    );

    return 0;
}
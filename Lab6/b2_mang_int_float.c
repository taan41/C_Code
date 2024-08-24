#include <stdio.h>

int main() {
    int int_arr[10] = {0};
    float float_arr[5] = {0};

    for(int i = 0; i < 5; i++) {
        printf("Nhap phan tu vi tri %d cua mang so thuc: ", i);
        scanf("%f", &float_arr[i]);
        int_arr[2*i + 1] = float_arr[i];
    }
    
    printf("Mang so nguyen:\n");
    for(int i = 0; i < 10; i++) printf("%d ", int_arr[i]);

    printf("\nMang so thuc:\n");
    for(int i = 0; i < 5; i++) printf("%.2f ", float_arr[i]);

}
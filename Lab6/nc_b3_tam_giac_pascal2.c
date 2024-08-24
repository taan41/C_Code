#include <stdio.h>

int main() {
    int rows = 5;
    // int arr[rows][rows];
    int arr[10][10];

    for(int i = 0; i < rows; i++) {
        // In khoang trang
        for(int space = 0; space < (rows - i - 1) * 2; space++) printf(" ");

        // Tinh va in cac phan tu
        for (int j = 0; j <= i; j++) {
            // Phan tu dau va cuoi = 1
            if(j == 0 || j == i) arr[i][j] = 1;
            else arr[i][j] = arr[i-1][j-1] + arr[i-1][j];

            printf("%d", arr[i][j]);
            
            if(j == i) printf("\n");
            else printf(" . ");
        }
    }
}

#include <stdio.h>
#include <stdlib.h>

void pascal(int rows);

int main() {
    int rows = 0;
    while(rows < 1) {
        printf("Nhap chieu cao cua tam giac Pascal (>0): ");
        scanf("%d", &rows);
    }

    pascal(rows);

    return 0;
}

void pascal(int rows) {
    int arr[rows][rows];

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
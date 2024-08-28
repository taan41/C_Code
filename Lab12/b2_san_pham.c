#include <stdio.h>
#include <stdlib.h>

typedef struct SanPham{
    char ma_SP[30];
    char ten_SP[30];
    int so_SP;
    float gia_SP;
}SanPham;

void flush() {
    while(getchar() != '\n');
}

void print_line(int len) {
    while(len-- > 0) putchar('-');
    putchar('\n');
}

int main() {
    int n;
    printf("Nhap n: ");
    scanf("%d", &n);
    flush();

    FILE *file = fopen("lab12_b2_san_pham.txt", "w");

    SanPham *danh_sach = malloc(n * sizeof(SanPham));
    for(int i = 0; i < n; i++) {
        fprintf(file, "San pham %d:\n", i + 1);

        printf("Nhap ma san pham %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].ma_SP);
        flush();
        fprintf(file, "Ma: %s\n", danh_sach[i].ma_SP);

        printf("Nhap ten san pham %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].ten_SP);
        flush();
        fprintf(file, "Ten: %s\n", danh_sach[i].ten_SP);

        printf("Nhap so luong san pham %d: ", i + 1);
        scanf("%d", &danh_sach[i].so_SP);
        flush();
        fprintf(file, "So luong: %d\n", danh_sach[i].so_SP);

        printf("Nhap gia san pham %d: ", i + 1);
        scanf("%f", &danh_sach[i].gia_SP);
        printf("\n");
        flush();
        fprintf(file, "Don gia: %.2f\n\n", danh_sach[i].gia_SP);
    }

    fclose(file);

    float tong = 0;
    printf("Thong tin cac san pham\n");
    print_line(30 * 2 + 8 + 13 * 2 + 6 + 2 * 5);
    printf("| %-30s | %-30s | %-8s | %-13s | %-13s |\n", "Ma san pham", "Ten san pham", "So luong", "Don gia ($)", "Tong tien ($)");
    print_line(30 * 2 + 8 + 13 * 2 + 6 + 2 * 5);
    for(int i = 0; i < n; i++) {
        printf("| %-30s | %-30s | %8d | %11.2f $ | %11.2f $ |\n", danh_sach[i].ma_SP, danh_sach[i].ten_SP, danh_sach[i].so_SP, danh_sach[i].gia_SP, danh_sach[i].gia_SP * danh_sach[i].so_SP);
        tong += danh_sach[i].gia_SP * danh_sach[i].so_SP;
    }
    print_line(30 * 2 + 8 + 13 * 2 + 6 + 2 * 5);
    printf("| %*.2f $ |\n", 30 * 2 + 8 + 13 * 2 + 6 + 2 * 5 - 6, tong);
    print_line(30 * 2 + 8 + 13 * 2 + 6 + 2 * 5);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct SanPham{
    char ma_SV[30];
    char ten_SV[30];
    char que_quan[30];
    char ghi_chu[30];
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

    FILE *file = fopen("lab12_b1_sinh_vien.txt", "w");

    SanPham *danh_sach = malloc(n * sizeof(SanPham));
    for(int i = 0; i < n; i++) {
        fprintf(file, "Sinh vien %d:\n", i + 1);

        printf("Nhap ma sinh vien %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].ma_SV);
        flush();
        fprintf(file, "Ma: %s\n", danh_sach[i].ma_SV);

        printf("Nhap ten sinh vien %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].ten_SV);
        flush();
        fprintf(file, "Ten: %s\n", danh_sach[i].ten_SV);

        printf("Nhap que quan cua sinh vien %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].que_quan);
        flush();
        fprintf(file, "Que quan: %s\n", danh_sach[i].que_quan);

        printf("Nhap ghi chu ve sinh vien %d: ", i + 1);
        scanf("%30[^\n]", danh_sach[i].ghi_chu);
        printf("\n");
        flush();
        fprintf(file, "Ghi chu: %s\n\n", danh_sach[i].ghi_chu);
    }

    fclose(file);

    printf("Thong tin cac sinh vien\n");
    print_line(30 * 4 + 5 + 2 * 4);
    printf("| %-30s | %-30s | %-30s | %-30s |\n", "Ma sinh vien", "Ten sinh vien", "Que quan", "Ghi chu");
    print_line(30 * 4 + 5 + 2 * 4);
    for(int i = 0; i < n; i++) {
        printf("| %-30s | %-30s | %-30s | %-30s |\n", danh_sach[i].ma_SV, danh_sach[i].ten_SV, danh_sach[i].que_quan, danh_sach[i].ghi_chu);
    }
    print_line(30 * 4 + 5 + 2 * 4);

    return 0;
}
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_ITEMS 100
#define MAX_NAME_LENGTH 50
#define MAX_CODE_LENGTH 10

// Cấu trúc hàng hóa
typedef struct {
    char ma_hang[MAX_CODE_LENGTH];
    char ten_hang[MAX_NAME_LENGTH];
    float don_gia;
} HangHoa;

HangHoa danh_sach[MAX_ITEMS];
int so_luong = 0;

// Hàm nhập danh sách hàng hóa
void nhap_danh_sach() {
    printf(" Nhap so luong hang hoa: ");
    scanf("%d", &so_luong);
    getchar();

    for (int i = 0; i < so_luong; i++) {
        printf("\n Nhap thong tin hang hoa thu %d:\n", i + 1);
        printf("  Ma hang: ");
        fgets(danh_sach[i].ma_hang, MAX_CODE_LENGTH, stdin);
        danh_sach[i].ma_hang[strcspn(danh_sach[i].ma_hang, "\n")] = 0;

        printf("  Ten hang: ");
        fgets(danh_sach[i].ten_hang, MAX_NAME_LENGTH, stdin);
        danh_sach[i].ten_hang[strcspn(danh_sach[i].ten_hang, "\n")] = 0;

        printf("  Don gia: ");
        scanf("%f", &danh_sach[i].don_gia);
        getchar();
    }
}

// Hàm hiển thị danh sách hàng hóa
void hien_thi_danh_sach() {
    printf(
        "\n Danh sach hang hoa:\n"
        "----------------------------------------------------------\n"
        " %-10s | %-30s | %-10s \n"
        "----------------------------------------------------------\n"
        , "Ma hang", "Ten hang", "Don gia"
    );
    for (int i = 0; i < so_luong; i++) {
        printf(" %-10s | %-30s | %-10.2f\n", danh_sach[i].ma_hang, danh_sach[i].ten_hang, danh_sach[i].don_gia);
    }
}

// Hàm lưu danh sách vào file
void luu_vao_file() {
    FILE *f = fopen("hang_hoa.txt", "w");

    for (int i = 0; i < so_luong; i++) {
        fprintf(f, "%s,%s,%.2f\n", danh_sach[i].ma_hang, danh_sach[i].ten_hang, danh_sach[i].don_gia);
    }

    printf(" Da luu danh sach vao hang_hoa.txt\n");

    fclose(f);
}

// Hàm xóa hàng hóa theo mã
void xoa_hang_hoa() {
    char ma_xoa[MAX_CODE_LENGTH];
    printf(" Nhap ma hang can xoa: ");
    fgets(ma_xoa, MAX_CODE_LENGTH, stdin);
    ma_xoa[strcspn(ma_xoa, "\n")] = 0;

    int vi_tri = -1;
    for (int i = 0; i < so_luong; i++) {
        if (strcmp(danh_sach[i].ma_hang, ma_xoa) == 0) {
            vi_tri = i;
            break;
        }
    }

    if (vi_tri == -1) {
        printf(" Khong tim thay hang hoa co ma %s\n", ma_xoa);
    } else {
        for (int i = vi_tri; i < so_luong - 1; i++) {
            danh_sach[i] = danh_sach[i + 1];
        }
        so_luong--;
        printf(" Da xoa hang hoa co ma %s\n", ma_xoa);
    }
}

int main() {
    int lua_chon;
    do {
        system("cls");

        printf(
            " MENU\n"
            "-------------------------------------\n"
            " 1. Nhap danh sach hang hoa\n"
            " 2. Hien thi danh sach hang hoa\n"
            " 3. Xoa hang hoa theo ma\n"
            " 4. Luu danh sach vao file\n"
            " 0. Thoat\n"
            "-------------------------------------\n"
            " Nhap lua chon cua ban: "
        );
        scanf("%d", &lua_chon);
        getchar();

        switch (lua_chon) {
            case 1:
                nhap_danh_sach();
                break;
            case 2:
                hien_thi_danh_sach();
                break;
            case 3:
                xoa_hang_hoa();
                break;
            case 4:
                luu_vao_file();
                break;
            case 0:
                printf(" Thoat chuong trinh.\n");
                break;
            default:
                printf(" Lua chon khong hop le.\n");
        }

        printf("\n Nhan phim bat ky de tiep tuc...\n");
        _getch();
    } while (lua_chon != 0);

    return 0;
}

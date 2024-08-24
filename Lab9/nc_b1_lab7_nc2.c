#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define UI_WIDTH 50

void arr_add_last();
void arr_mod_pos();
void arr_del_pos();
void arr_find_itr();
void arr_find_bin();
void arr_sort();
void arr_add_sorted();
void arr_prnt();

int binary_search(int target, int is_exact);
void quick_sort(int l, int r);

void realloc_arr(int size_increment);
int is_empty();
int is_sorted();
int scn_value();
int scn_pos();

void prnt_line(size_t length, char end);
void prnt_menu();
void flush();

size_t ARR_SIZE = 0, BUFFER_SIZE = 10;
int *ARR, IS_SORTED = 0;

int main() {
    ARR = malloc(BUFFER_SIZE * sizeof(int));

    int menu_choice;

    system("cls");
    prnt_menu();

    while(1) {
        scanf("%d", &menu_choice);

        switch(menu_choice) {
            case 1:
                arr_add_last();
                goto return_menu_label;

            case 2:
                arr_mod_pos();
                goto return_menu_label;

            case 3:
                arr_del_pos();
                goto return_menu_label;

            case 4:
                arr_find_itr();
                goto return_menu_label;

            
            case 5:
                arr_find_bin();
                goto return_menu_label;

            case 6:
                arr_sort();
                goto return_menu_label;

            case 7:
                arr_add_sorted();
                goto return_menu_label;

            case 8:
                arr_prnt();

            return_menu_label:
                prnt_line(UI_WIDTH, '-');
                printf("Nhan phim bat ky de tiep tuc...");
                _getch();
                system("cls");
                prnt_menu();
                break;

            case 0:
                printf("Thoat chuong trinh...\n");
                exit(0);
                break;

            default:
                printf("Khong hop le, vui long nhap lai: ");
                break;
        }
    }

    free(ARR);
    return 0;
}

void arr_add_last() {
    int add_amount;
    printf("Nhap so phan tu muon nhap: ");
    scanf("%d", &add_amount);

    while(add_amount-- > 0) {
        realloc_arr(1);
        ARR[ARR_SIZE - 1] = scn_value();
    }

    IS_SORTED = 0;
}

void arr_mod_pos() {
    if(is_empty()) return;

    int pos = scn_pos();
    if(pos == -1) return;
    ARR[pos] = scn_value();

    IS_SORTED = 0;
}

void arr_del_pos() {
    if(is_empty()) return;

    int pos = scn_pos();
    if(pos == -1) return;

    memmove(ARR + pos, ARR + pos + 1, (ARR_SIZE - pos - 1) * sizeof(int));
    realloc_arr(-1);
}

void arr_find_itr() {
    if(is_empty()) return;

    int value = scn_value(), pos = -1;
    for(int i = 0; i < ARR_SIZE; i++) if(ARR[i] == value) {
        pos = i;
        break;
    }

    if(pos >= 0) printf("Tim thay %d tai vi tri (tinh tu 0): %d\n", value, pos);
    else printf("Khong tim thay phan tu.\n");
}

void arr_find_bin() {
    if(is_empty()) return;
    if(!is_sorted()) return;

    int value = scn_value();
    int pos = binary_search(value, 1);

    if(pos >= 0) printf("Tim thay %d tai vi tri (tinh tu 0): %d\n", value, pos);
    else printf("Khong tim thay phan tu.\n");
}

void arr_sort() {
    if(is_empty()) return;

    printf("Da sap xep mang.\n");
    if(IS_SORTED) return;
    quick_sort(0, ARR_SIZE - 1);
    IS_SORTED = 1;
}

void arr_add_sorted() {
    if(is_empty()) return;
    if(!is_sorted()) return;

    int add_amount;
    printf("Nhap so phan tu muon nhap: ");
    scanf("%d", &add_amount);

    while(add_amount-- > 0) {
        int value = scn_value();
        int pos = binary_search(value, 0);

        realloc_arr(1);
        memmove(ARR + pos + 1, ARR + pos, (ARR_SIZE - pos - 1) * sizeof(int));
        ARR[pos] = value;
    }
}

void arr_prnt() {
    if(is_empty()) return;

    printf("Cac phan tu cua mang:\n");
    for(int i = 0; i < ARR_SIZE; i++) {
        if(i % 10 == 0 && i) putchar('\n');
        printf("%d ", ARR[i]);
    }
    putchar('\n');
}

int binary_search(int target, int is_exact) {
    int l = 0, r = ARR_SIZE - 1, pos = is_exact ? -1 : 0;

    while(l <= r) {
        int mid = (l + r) / 2;

        if(ARR[mid] == target) {
            pos = mid;
            l = mid + 1;
        }
        else if (ARR[mid] > target) {
            l = mid + 1;

            if(!is_exact) {
                if(mid + 1 == ARR_SIZE) return mid + 1;
                else if(ARR[mid + 1] < target) return mid + 1;
            }
        }
        else {
            r = mid - 1;

            if(!is_exact) {
                if(mid == 0) return mid;
                else if(ARR[mid - 1] > target) return mid;
            }
        }
    }

    return pos;
}

int quick_sort_partition(int l, int r) {
    // Chon pivot la phan tu cuoi cung ben phai
    int pivot = ARR[r];
    int i = l - 1;
    int temp;

    // Hoan doi cac phan tu lon hon pivot sang ben trai
    for(int j = l; j < r; j++) {
        if(ARR[j] >= pivot) {
            i++;

            temp = ARR[j];
            ARR[j] = ARR[i];
            ARR[i] = temp;
        }
    }

    // Swap pivot vao giua, cac phan tu lon hon o ben trai, be hon o ben phai
    temp = ARR[i + 1];
    ARR[i + 1] = ARR[r];
    ARR[r] = temp;

    // Tra ve vi tri pivot
    return i + 1;
}

void quick_sort(int l, int r) {
    if(l < r) {
        int pivot_pos = quick_sort_partition(l, r);

        // Tiep tuc chia & xep mang ben phai (lon hon pivot) va ben trai (be hon pivot)
        quick_sort(l, pivot_pos - 1);
        quick_sort(pivot_pos + 1, r);
    }
}

void realloc_arr(int size_increment) {
    ARR_SIZE += size_increment;
    if(ARR_SIZE >= BUFFER_SIZE) {
        BUFFER_SIZE *= 2;
        ARR = realloc(ARR, BUFFER_SIZE * sizeof(int));
    }
}

int is_empty() {
    if(ARR_SIZE == 0) printf("Mang rong, them phan tu vao mang truoc.\n");
    return ARR_SIZE == 0;
}

int is_sorted() {
    if(!IS_SORTED) printf("Can sap xep mang truoc.\n");
    return IS_SORTED;
}

int scn_value() {
    int value;
    printf("Nhap gia tri phan tu: ");
    scanf("%d", &value);
    return value;
}

int scn_pos() {
    int pos;
    printf("Nhap vi tri phan tu (tinh tu 0): ");
    scanf("%d", &pos);

    if(pos < 0 || pos >= ARR_SIZE) {
        printf("Vi tri khong hop le.\n");
        return -1;
    }
    return pos;
}

void prnt_line(size_t length, char end) {
    putchar(end);
    while(length-- > 2) putchar('-');
    printf("%c\n", end);
}

void prnt_menu() {
    prnt_line(UI_WIDTH, '+');
    printf("%*c\r%*s\r|\n", UI_WIDTH, '|', (UI_WIDTH + 4) / 2, "MENU");
    prnt_line(UI_WIDTH, '+');

    printf(
        "%*c\r| 1. Nhap them mot phan tu cho mang\n"
        "%*c\r| 2. Sua phan tu theo vi tri cua mang\n"
        "%*c\r| 3. Xoa phan tu mang theo vi tri\n"
        "%*c\r| 4. Tim kiem tuyen tinh mot phan tu trong mang\n"
        "%*c\r| 5. Tim kiem nhi phan mot phan tu trong mang\n"
        "%*c\r| 6. Sap xep theo chieu giam dan\n"
        "%*c\r| 7. Chen them mot phan tu cho mang sao cho\n"
        "%*c\r|    danh sach sap xep van duoc bao toan\n"
        "%*c\r| 8. Hien thi mang\n"
        "%*c\r| 0. Thoat chuong trinh\n",
        UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|',
        UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|', UI_WIDTH, '|'
    );

    prnt_line(UI_WIDTH, '+');
    printf("Moi ban nhap: ");
}

void flush() {
    char ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

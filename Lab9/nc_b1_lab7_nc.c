#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void arr_add_last(int **arr, size_t *sz_arr);
void arr_mod_pos(int **arr, size_t *sz_arr);
void arr_del_pos(int **arr, size_t *sz_arr);
void arr_find_itr(int **arr, size_t *sz_arr);
void arr_find_bin(int **arr, size_t *sz_arr);
void arr_add_sorted(int **arr, size_t *sz_arr);
void arr_prnt(int **arr, size_t *sz_arr);

int scn_value();
int scn_pos();
void prnt_line(size_t length);
void prnt_menu();

int binary_search(int *arr, size_t arr_size, int target, int is_exact);
void quick_sort(int **arr, int l, int r);

const char *options[] = {
    "Nhap them mot phan tu cho mang",
    "Sua phan tu theo vi tri cua mang",
    "Xoa phan tu mang theo vi tri",
    "Tim kiem tuyen tinh mot phan tu trong mang",
    "Tim kiem nhi phan mot phan tu trong mang",
    "Sap xep theo chieu giam dan",
    "Chen them mot phan tu cho mang sao cho danh sach sap xep van duoc bao toan",
    "Hien thi mang",
    "Thoat chuong trinh",
    "Hien thi lai MENU",
};
const size_t sz_opt = sizeof(options) / sizeof(options[0]);
const size_t sz_int = sizeof(int);

int main() {
    prnt_menu();
    prnt_line(50);

    size_t sz_arr = 0;
    int input, *arr;
    while(1) {
        printf("Moi ban nhap: ");
        scanf("%d", &input);
        if(input > 0 && input < sz_opt - 1) printf("%s:\n", options[input - 1]);

        switch(input) {
            // Them phan tu
            case 1:
                arr_add_last(&arr, &sz_arr);
                break;

            // Sua theo vi tri
            case 2:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                arr_mod_pos(&arr, &sz_arr);
                break;

            // Xoa theo vi tri
            case 3:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                arr_del_pos(&arr, &sz_arr);
                break;

            // Tim tuyen tinh (for loop)
            case 4:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                arr_find_itr(&arr, &sz_arr);
                break;

            // Tim nhi phan (binary)
            case 5:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                arr_find_bin(&arr, &sz_arr);
                break;

            // Sap xep giam dan
            case 6:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                quick_sort(&arr, 0, sz_arr - 1);
                break;

            // Them phan tu theo sap xep
            case 7:
                arr_add_sorted(&arr, &sz_arr);
                break;

            // In mang
            case 8:
                if(sz_arr == 0) {
                    printf("Mang rong, them phan tu vao mang truoc.\n");
                    break;
                }
                arr_prnt(&arr, &sz_arr);
                break;

            // Ket thuc
            case 0:
                printf("Ket thuc chuong trinh...\n");
                free(arr);
                exit(0);
                break;

            // In menu
            case -1:
                prnt_menu();
                break;

            default:
                printf("Khong hop le, vui long nhap lai.\n");
                break;
        }
        prnt_line(50);
    }

    return 0;
}

void arr_add_last(int **arr, size_t *sz_arr) {
    int value = scn_value();
    if(++(*sz_arr) == 1) *arr = (int *)calloc(*sz_arr, sz_int);
    else *arr = (int *)realloc(*arr, *sz_arr * sz_int);
    (*arr)[*sz_arr - 1] = value;
}

void arr_mod_pos(int **arr, size_t *sz_arr) {
    int pos = scn_pos();
    if(pos < 0 || pos >= *sz_arr) {
        printf("Vi tri khong hop le.\n");
        return;
    }
    int value = scn_value();
    (*arr)[pos] = value;
}

void arr_del_pos(int **arr, size_t *sz_arr) {
    int pos = scn_pos();
    if(pos < 0 || pos >= *sz_arr) {
        printf("Vi tri khong hop le.\n");
        return;
    }
    memmove(*arr + pos, *arr + pos + 1, (*sz_arr - pos - 1) * sz_int);
    *arr = realloc(*arr, --(*sz_arr) * sz_int);
}

void arr_find_itr(int **arr, size_t *sz_arr) {
    int value = scn_value(), pos = -1;
    for(int i = 0; i < *sz_arr; i++) if((*arr)[i] == value) {
        pos = i;
        break;
    }

    if(pos >= 0) printf("Tim thay %d tai vi tri (tinh tu 0): %d\n", value, pos);
    else printf("Khong tim thay phan tu.\n");
}

void arr_find_bin(int **arr, size_t *sz_arr) {
    printf("(Can sap xep mang truoc khi tim.)\n");
    int value = scn_value();
    int pos = binary_search(*arr, *sz_arr, value, 1);

    if(pos >= 0) printf("Tim thay %d tai vi tri (tinh tu 0): %d\n", value, pos);
    else printf("Khong tim thay phan tu.\n");
}

void arr_add_sorted(int **arr, size_t *sz_arr) {
    int value = scn_value();
    int pos = binary_search(*arr, *sz_arr, value, 0);

    if(++(*sz_arr) == 1) *arr = (int *)malloc(*sz_arr * sz_int);
    else *arr = realloc(*arr, *sz_arr * sz_int);

    if(pos + 1 < *sz_arr) memmove(*arr + pos + 1, *arr + pos, (*sz_arr - pos - 1) * sz_int);
    (*arr)[pos] = value;
}

void arr_prnt(int **arr, size_t *sz_arr) {
    printf("Cac phan tu cua mang:\n");
    for(int i = 0; i < *sz_arr; i++) printf("%d ", (*arr)[i]);
    printf("\n");
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
    return pos;
}

void prnt_line(size_t length) {
    printf("+");
    while(length-- > 2) printf("-");
    printf("+\n");
}

void prnt_menu() {
    prnt_line(50);
    printf("|%23s%s%23s|\n", " ", "MENU", " ");
    prnt_line(50);

    for(int i = 1; i < sz_opt - 1 ; i++) {
        int pad_space = 4 + (int) log10(i);
        int pad_str = 46 - (int) log10(i);
        int line = strlen(options[i-1]) / pad_str;

        if(line > 0) {
            int space_i = pad_str - 1;
            while(options[i-1][space_i] != ' ') space_i--;
            if(space_i < 0) space_i = pad_str - 1;

            char sub_str[space_i + 2];
            strncpy(sub_str, options[i-1], space_i + 1);
            sub_str[space_i + 1] = '\0';

            printf("| %d. %-*s|\n", i, pad_str, sub_str);

            while(line-- > 0) {
                int last_space_i = ++space_i;
                space_i += pad_str;
                while(options[i-1][space_i] != ' ') space_i--;
                if(space_i < last_space_i) space_i = last_space_i + pad_str - 1;

                strncpy(sub_str, options[i-1] + last_space_i, space_i - last_space_i + 1);
                sub_str[space_i - last_space_i + 1] = '\0';
                printf("|%*s%-*s|\n", pad_space, " ", pad_str, sub_str);
            }
            continue;
        }
        printf("| %d. %-*s|\n", i, pad_str, options[i-1]);
    }
    printf("| 0. %-46s|\n", options[sz_opt - 2]);
    printf("| -1. %-45s|\n", options[sz_opt - 1]);
}

int binary_search(int *arr, size_t sz_arr, int target, int is_exact) {
    int l = 0, r = sz_arr - 1, result = is_exact ? -1 : 0;

    while(l <= r) {
        int mid = (l + r) / 2;

        if(arr[mid] == target) {
            result = mid;
            l = mid + 1;
        }
        else if (arr[mid] > target) {
            l = mid + 1;
            if(!is_exact) {
                if(mid + 1 >= sz_arr) return mid + 1;
                else if(arr[mid + 1] < target) return mid + 1;
            }
        }
        else {
            r = mid - 1;
            if(!is_exact) {
                if(mid - 1 < 0) return mid;
                else if(arr[mid - 1] > target) return mid;
            }
        }
    }

    return result;
}

int quick_sort_partition(int **arr, int l, int r) {
    int pivot = (*arr)[r];
    int i = l - 1;
    int temp;

    for(int j = l; j < r; j++) {
        if((*arr)[j] >= pivot) {
            i++;

            temp = (*arr)[j];
            (*arr)[j] = (*arr)[i];
            (*arr)[i] = temp;
        }
    }

    temp = (*arr)[i + 1];
    (*arr)[i + 1] = (*arr)[r];
    (*arr)[r] = temp;

    return i + 1;
}

void quick_sort(int **arr, int l, int r) {
    if(l < r) {
        int pivot_pos = quick_sort_partition(arr, l, r);

        quick_sort(arr, l, pivot_pos - 1);
        quick_sort(arr, pivot_pos + 1, r);
    }
}

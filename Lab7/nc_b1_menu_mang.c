#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void prnt_menu();
void prnt_line(int length);
void scn_value(int *value);
void scn_pos(int *pos);
int binary_search(int arr[], int arr_size, int target, int is_exact);
void quick_sort(int arr[], int l, int r);

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

int main() {
    prnt_menu();
    prnt_line(50);

    size_t arr_size = 0, sz_int = sizeof(int);
    int input, value, pos;
    int *arr = malloc(arr_size * sz_int);
    for(;;) {
        printf("Moi ban nhap: ");
        scanf("%d", &input);
        if(input > 0 && input < sz_opt - 1) printf("%s:\n", options[input - 1]);

        switch(input) {
            // Them phan tu
            case 1:
                scn_value(&value);
                if(arr_size == 0) arr = (int *)malloc(++arr_size * sz_int);
                else arr = realloc(arr, ++arr_size * sz_int);
                arr[arr_size - 1] = value;
                break;

            // Sua theo vi tri
            case 2:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                scn_pos(&pos);
                scn_value(&value);
                arr[pos] = value;
                break;

            // Xoa theo vi tri
            case 3:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                scn_pos(&pos);
                memmove(arr + pos, arr + pos + 1, (arr_size - pos - 1) * sz_int);
                arr = realloc(arr, --arr_size * sz_int);
                break;

            // Tim tuyen tinh (for loop)
            case 4:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                scn_value(&value);
                pos = -1;
                for(int i = 0; i < arr_size; i++) if(arr[i] == value) {
                    pos = i;
                    break;
                }
                if(pos >= 0) printf("Phan tu duoc tim thay tai vi tri: %d\n", pos);
                else printf("Khong tim thay phan tu.\n");
                break;

            // Tim nhi phan (binary)
            case 5:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                scn_value(&value);
                pos = binary_search(arr, arr_size, value, 1);
                if(pos >= 0) printf("Phan tu duoc tim thay tai vi tri: %d\n", pos);
                else printf("Khong tim thay phan tu.\n");
                break;

            // Sap xep giam dan
            case 6:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                quick_sort(arr, 0, arr_size - 1);
                break;

            // Them phan tu theo sap xep
            case 7:
                scn_value(&value);
                pos = binary_search(arr, arr_size, value, 0);
                if(arr_size == 0) arr = (int *)malloc(++arr_size * sz_int);
                else arr = realloc(arr, ++arr_size * sz_int);
                printf("pos: %d size: %d\n", pos, arr_size);
                if(pos + 1 < arr_size) memmove(arr + pos + 1, arr + pos, (arr_size - pos - 1) * sz_int);
                arr[pos] = value;

            // In mang
            case 8:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                printf("Cac phan tu cua mang:\n");
                for(int i = 0; i < arr_size; i++) printf("%d ", arr[i]);
                printf("\n");
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

void scn_value(int *value) {
    printf("Nhap gia tri phan tu: ");
    scanf("%d", value);
}

void scn_pos(int *pos) {
    printf("Nhap vi tri phan tu: ");
    scanf("%d", pos);
}

void prnt_line(int length) {
    printf("+");
    for(int i = 0; i < length; i++) printf("-");
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

int binary_search(int arr[], int sz_arr, int target, int is_exact) {
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

int quick_sort_partition(int arr[], int l, int r) {
    int pivot = arr[r];
    int i = l - 1;
    int temp;

    for(int j = l; j < r; j++) {
        if(arr[j] >= pivot) {
            i++;

            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }

    temp = arr[i + 1];
    arr[i + 1] = arr[r];
    arr[r] = temp;

    return i + 1;
}

void quick_sort(int arr[], int l, int r) {
    if(l < r) {
        int pivot_pos = quick_sort_partition(arr, l, r);

        quick_sort(arr, l, pivot_pos - 1);
        quick_sort(arr, pivot_pos + 1, r);
    }
}

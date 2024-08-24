#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_WIDTH 50

void prnt_line(size_t length);
void prnt_text(size_t width, char *text, int mode);
void prnt_menu();

int main() {
    prnt_menu();

    int input, value, pos, *arr;
    size_t arr_size = 0;
    for(;;) {
        printf("Moi ban nhap: ");
        scanf("%d", &input);
        system("cls");
        prnt_menu();

        switch(input) {
            // Them 1 phan tu vao cuoi mang
            case 1:
                if(!arr_size++) arr = malloc(arr_size * sizeof(int));
                else arr = realloc(arr, arr_size * sizeof(int));
                
                printf("Nhap gia tri phan tu: ");
                scanf("%d", &arr[arr_size - 1]);
                break;

            // Sua theo vi tri
            case 2:
                if(!arr_size) {
                    printf("Mang rong.\n");
                    break;
                }
                
                printf("Nhap vi tri phan tu: ");
                scanf("%d", &pos);
                if(pos < 0 || pos >= arr_size) {
                    printf("Vi tri khong hop le.\n");
                    break;
                }

                printf("Nhap gia tri phan tu: ");
                scanf("%d", &arr[pos]);
                break;

            // Xoa theo vi tri
            case 3:
                if(!arr_size) {
                    printf("Mang rong.\n");
                    break;
                }
                
                printf("Nhap vi tri phan tu: ");
                scanf("%d", &pos);
                if(pos < 0 || pos >= arr_size) {
                    printf("Vi tri khong hop le.\n");
                    break;
                }
                
                memmove(arr + pos, arr + pos + 1, (arr_size - pos - 1) * sizeof(int));
                arr = realloc(arr, --arr_size * sizeof(int));
                break;

            // Tim tuyen tinh (for loop)
            case 4:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }

                printf("Nhap du lieu can tim: ");
                scanf("%d", &value);

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

                printf("Tim chinh xac hon sau khi sap xep mang.\nNhap du lieu can tim: ");
                scanf("%d", &value);

                pos = -1;
                for(int l = 0, r = arr_size - 1; l <= r;) {
                    int mid = (l + r) / 2;

                    if(arr[mid] == value) {
                        pos = mid;
                        l = mid + 1;
                    } else if(arr[mid] > value) l = mid + 1;
                    else r = mid - 1;
                }
                
                if(pos >= 0) printf("Phan tu duoc tim thay tai vi tri: %d\n", pos);
                else printf("Khong tim thay phan tu.\n");
                break;

            // Sap xep giam dan (selection sort)
            case 6:
                if(arr_size == 0) {
                    printf("Mang rong.\n");
                    break;
                }
                printf("Da sap xep mang theo chieu giam dan.\n");

                for(int i = 0, max, temp; i < arr_size; i++) {
                    max = i;

                    for(int j = i + 1; j < arr_size - i; j++) if(arr[max] < arr[j]) max = j;

                    if(max != i) {
                        temp = arr[max];
                        arr[max] = arr[i];
                        arr[i] = temp;
                    }
                }
                break;

            // Them phan tu theo sap xep
            case 7:
                if(!arr_size++) arr = malloc(arr_size * sizeof(int));
                else arr = realloc(arr, arr_size * sizeof(int));
                
                printf("Nhap chinh xac hon sau khi sap xep mang.\nNhap gia tri phan tu: ");
                scanf("%d", &value);

                pos = -1;
                for(int l = 0, r = arr_size - 1; l <= r;) {
                    int mid = (l + r) / 2;

                    if(arr[mid] == value) {
                        pos = mid;
                        l = mid + 1;
                    } else if(arr[mid] < value) {
                        pos = mid;
                        r = mid - 1;
                    }
                    else l = mid + 1;
                }
                if(pos < 0) pos = 0;
                else if(pos >= arr_size) pos = arr_size - 1;

                memmove(arr + pos + 1, arr + pos, (arr_size - pos - 1) * sizeof(int));
                arr[pos] = value;
                break;

            // In mang
            case 8:
                if(!arr_size) {
                    printf("Mang rong.\n");
                    break;
                }

                printf("Cac phan tu cua mang:");
                for(int i = 0; i < arr_size; i++) {
                    if(i % 10 == 0) printf("\n");
                    printf("%d ", arr[i]);
                }
                printf("\n");
                break;

            case 0:
                printf("Thoat chuong trinh...");
                free(arr);
                exit(0);
                break;

            default:
                printf("Khong hop le, hay nhap lai.\n");
                break;
        }
        
        prnt_line(MENU_WIDTH);
    }

}

void prnt_line(size_t length) {
    printf("+");
    while(length-- > 0) printf("-");
    printf("+\n");
}

void prnt_text(size_t width, char *text, int mode) {
    size_t text_length = strlen(text);
    size_t pad_length = (width - text_length) / 2;

    switch(mode) {
        case 0:
            printf("| %-*s |\n", width - 2, text);
            break;

        case 1:
            printf("| %*s |\n", width - 2, text);
            break;

        case 2:
            printf("|%*s%s%*s|\n", pad_length, " ", text, width - pad_length - text_length, " ");
            break;
    }
}

void prnt_menu() {
    prnt_line(MENU_WIDTH);
    prnt_text(MENU_WIDTH, "MENU", 2);
    prnt_line(MENU_WIDTH);

    prnt_text(MENU_WIDTH, "1. Nhap them mot phan tu cho mang", 0);
    prnt_text(MENU_WIDTH, "2. Sua phan tu theo vi tri cua mang", 0);
    prnt_text(MENU_WIDTH, "3. Xoa phan tu mang theo vi tri", 0);
    prnt_text(MENU_WIDTH, "4. Tim kiem tuyen tinh mot phan tu trong mang", 0);
    prnt_text(MENU_WIDTH, "5. Tim kiem nhi phan mot phan tu trong mang", 0);
    prnt_text(MENU_WIDTH, "6. Sap xep theo chieu giam dan", 0);
    prnt_text(MENU_WIDTH, "7. Chen them mot phan tu cho mang sao cho", 0);
    prnt_text(MENU_WIDTH, "   danh sach sap xep van duoc bao toan", 0);
    prnt_text(MENU_WIDTH, "8. Hien thi mang", 0);
    prnt_text(MENU_WIDTH, "0. Thoat chuong trinh", 0);

    prnt_line(MENU_WIDTH);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MENU_WIDTH 50
#define PROD_NAME_LENGTH 30
#define PROD_QUANTITY_WIDTH 8
#define PROD_PRICE_WIDTH 13
#define LIST_WIDTH PROD_NAME_LENGTH + PROD_QUANTITY_WIDTH + PROD_PRICE_WIDTH * 2 + 19

typedef struct Product{
    char name[PROD_NAME_LENGTH];
    int quantity;
    int price;
}Product;

int compare_by_name(const void *a, const void *b);
int compare_by_price(const void *a, const void *b);

char *to_lower_str(char *str);

void find_prod(Product *products, size_t products_size);

void prnt_line(size_t length, char side);
void prnt_text(size_t width, char *text, int mode);
void prnt_menu();
void prnt_prod_list(Product *products, size_t products_size, char* title);
void flush();

int main() {
    prnt_menu();

    int input;
    Product *products;
    size_t products_size = 0;
    for(;;) {
        printf("Moi ban nhap: ");
        scanf("%d", &input);
        flush();
        system("cls");
        prnt_menu();

        switch(input) {
            case 1:
                if(products_size == 0) products = malloc(sizeof(Product));
                else products = realloc(products, (products_size + 1) * sizeof(Product));

                printf("Nhap ten san pham: ");
                fgets(products[products_size].name, PROD_NAME_LENGTH, stdin);
                products[products_size].name[strcspn(products[products_size].name, "\n")] = '\0';
                
                printf("Nhap so luong san pham: ");
                scanf("%d", &products[products_size].quantity);
                
                printf("Nhap gia san pham: ");
                scanf("%d", &products[products_size].price);
                
                products_size++;
                prnt_line(MENU_WIDTH, '-');
                flush();
                break;

            case 2:
                qsort(products, products_size, sizeof(Product), compare_by_name);
                prnt_prod_list(products, products_size, "DANH MUC SAN PHAM");
                break;

            case 3:
                qsort(products, products_size, sizeof(Product), compare_by_price);
                prnt_prod_list(products, products_size, "DANH MUC SAN PHAM");
                break;

            case 4:
                find_prod(products, products_size);
                break;

            case 0:
                printf("Thoat chuong trinh...");
                free(products);
                exit(0);
                break;

            default:
                printf("Khong hop le, hay nhap lai.\n");
                break;
        }
    }

    return 0;
}

int compare_by_name(const void *a, const void *b) {
    Product *prodA = (Product *)a;
    Product *prodB = (Product *)b;

    return strcmp(prodA->name, prodB->name);
}

int compare_by_price(const void *a, const void *b) {
    Product *prodA = (Product *)a;
    Product *prodB = (Product *)b;

    return (*prodA).price - (*prodB).price;
}

char *to_lower_str(char *str) {
    char *str_lowered = strdup(str);
    for(int i = 0; i < strlen(str_lowered); str_lowered[i] = tolower(str_lowered[i++]));
    return str_lowered;
}

void find_prod(Product *products, size_t products_size) {
    printf("Nhap ten can tim: ");

    char name_to_find[PROD_NAME_LENGTH];
    fgets(name_to_find, PROD_NAME_LENGTH, stdin);
    name_to_find[strcspn(name_to_find, "\n")] = '\0';

    Product *products_found;
    size_t products_found_size = 0;
    for(int i = 0; i < products_size; i++) {
        if(strstr(to_lower_str(products[i].name), to_lower_str(name_to_find)) != NULL) {
            if(!products_found_size++) products_found = malloc(sizeof(Product));
            else products_found = realloc(products_found, products_found_size * sizeof(Product));
            products_found[products_found_size - 1] = products[i];
        }
    }

    char *title_temp = "CAC SAN PHAM CO TEN CHUA ";
    char title[strlen(title_temp) + strlen(name_to_find)];
    strcpy(title, title_temp);
    strcat(title, name_to_find);

    prnt_prod_list(products_found, products_found_size, title);
    free(products_found);
}

void prnt_line(size_t length, char side) {
    printf("%c", side);
    while(length-- > 2) printf("-");
    printf("%c\n", side);
}

void prnt_text(size_t width, char *text, int mode) {
    size_t text_length = strlen(text);
    size_t pad_length = (width - text_length - 2) / 2;

    switch(mode) {
        case 0:
            printf("| %-*s |\n", width - 4, text);
            break;

        case 1:
            printf("| %*s |\n", width - 4, text);
            break;

        case 2:
            printf("|%*s%s%*s|\n", pad_length, " ", text, width - pad_length - text_length - 2, " ");
            break;
    }
}

void prnt_menu() {
    prnt_line(MENU_WIDTH, '+');
    prnt_text(MENU_WIDTH, "MENU", 2);
    prnt_line(MENU_WIDTH, '+');

    prnt_text(MENU_WIDTH, "1. Them san pham", 0);
    prnt_text(MENU_WIDTH, "2. Hien thi san pham theo ten tang dan", 0);
    prnt_text(MENU_WIDTH, "3. Hien thi san pham theo gia tang dan", 0);
    prnt_text(MENU_WIDTH, "4. Tim kiem san pham theo ten", 0);
    prnt_text(MENU_WIDTH, "0. Thoat chuong trinh", 0);

    prnt_line(MENU_WIDTH, '+');
}

void prnt_prod_list(Product *products, size_t products_size, char* title) {
    prnt_line(LIST_WIDTH, '+');
    prnt_text(LIST_WIDTH, title, 2);
    prnt_line(LIST_WIDTH, '+');

    printf(
        "| STT | %-*s | %*s | %*s | %*s |\n",
        PROD_NAME_LENGTH, "Ten san pham",
        PROD_QUANTITY_WIDTH, "So luong",
        PROD_PRICE_WIDTH, "Don gia ($)",
        PROD_PRICE_WIDTH, "Tong tien ($)"
    );
    prnt_line(LIST_WIDTH, '|');

    int total = 0;
    for(int i = 0; i < products_size; i++) {
        printf(
            "| %03d | %-*s | %*d | %*d | %*d |\n",
            i+1, PROD_NAME_LENGTH, products[i].name,
            PROD_QUANTITY_WIDTH, products[i].quantity,
            PROD_PRICE_WIDTH, products[i].price,
            PROD_PRICE_WIDTH, products[i].quantity * products[i].price
        );

        total += products[i].quantity * products[i].price;
    }

    prnt_line(LIST_WIDTH, '|');
    printf("|%*d |\n", LIST_WIDTH - 3, total);
    prnt_line(LIST_WIDTH, '+');
}

void flush() {
    char buffer;
    while((buffer = getchar()) != '\n' && buffer != EOF);
}

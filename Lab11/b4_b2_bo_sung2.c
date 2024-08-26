#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define NAME_LEN 30
#define QUANTITY_LEN 8
#define PRICE_LEN 13

#define UI_WIDTH 50
#define LIST_WIDTH NAME_LEN + QUANTITY_LEN + PRICE_LEN * 2 + 19

typedef struct Product{
    char name[NAME_LEN + 1];
    int quantity;
    float price;
}Product;

void add_prod();
void find_prod();
void prnt_prod_list(Product *list_to_prnt, size_t size, char* title);

int compare_by_name(const void *a, const void *b);
int compare_by_price(const void *a, const void *b);
char *dup_lowered_str(char *str);

void prnt_line(size_t length, char side);
void prnt_text(size_t width, char *text, int mode);
void prnt_menu();
void flush();

static Product *products;
static size_t products_size = 0, buffer_size = 10;

int main() {
    products = malloc(buffer_size * sizeof(Product));

    int choice;
    while(1) {
        system("cls");
        prnt_menu();
        printf("Moi ban nhap: ");
        scanf("%d", &choice);
        flush();

        switch(choice) {
            case 1: add_prod(); break;

            case 2:
                if(products_size == 0) {
                    printf("Chua co san pham trong danh sach\n");
                    break;
                }
                qsort(products, products_size, sizeof(Product), compare_by_name);
                prnt_prod_list(products, products_size, "DANH MUC SAN PHAM THEO TEN TANG DAN");
                break;

            case 3:
                if(products_size == 0) {
                    printf("Chua co san pham trong danh sach\n");
                    break;
                }
                qsort(products, products_size, sizeof(Product), compare_by_price);
                prnt_prod_list(products, products_size, "DANH MUC SAN PHAM THEO GIA TANG DAN");
                break;

            case 4:
                find_prod();
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

        printf("Nhan phim bat ky de tiep tuc...");
        _getch();
    }

    return 0;
}

void add_prod() {
    if(products_size >= buffer_size){
        buffer_size *= 2;
        products = realloc(products, buffer_size * sizeof(Product));
    }

    int amount;
    printf("Nhap so san pham muon them: ");
    scanf("%d", &amount);
    flush();

    while(amount-- > 0) {
        putchar('\n');

        printf("Nhap ten san pham: ");
        fgets(products[products_size].name, NAME_LEN, stdin);
        products[products_size].name[strcspn(products[products_size].name, "\n")] = '\0';
        
        printf("Nhap so luong: ");
        scanf("%d", &products[products_size].quantity);
        
        printf("Nhap don gia ($): ");
        scanf("%f", &products[products_size].price);

        products_size++;
        flush();
    }
    
    prnt_line(UI_WIDTH, '-');
}

void find_prod() {
    printf("Nhap ten can tim: ");

    char name_to_find[NAME_LEN + 1];
    fgets(name_to_find, NAME_LEN, stdin);
    name_to_find[strcspn(name_to_find, "\n")] = '\0';

    Product *products_found = malloc(sizeof(Product));
    size_t products_found_size = 0;

    for(int i = 0; i < products_size; i++) {
        if(strstr(dup_lowered_str(products[i].name), dup_lowered_str(name_to_find)) != NULL) {
            products_found = realloc(products_found, (products_found_size + 1) * sizeof(Product));

            products_found[products_found_size++] = products[i];
        }
    }

    char title_template[] = "CAC SAN PHAM CO TEN CHUA";
    char title[strlen(title_template) + strlen(name_to_find) + 1];
    sprintf(title, "%s %s", title_template, name_to_find);

    prnt_prod_list(products_found, products_found_size, title);
    free(products_found);
}

void prnt_prod_list(Product *list_to_prnt, size_t size, char* title) {
    prnt_line(LIST_WIDTH, '+');
    prnt_text(LIST_WIDTH, title, 2);
    prnt_line(LIST_WIDTH, '+');

    printf(
        "| STT | %-*s | %*s | %*s | %*s |\n",
        NAME_LEN, "Ten san pham",
        QUANTITY_LEN, "So luong",
        PRICE_LEN, "Don gia ($)",
        PRICE_LEN, "Tong tien ($)"
    );
    prnt_line(LIST_WIDTH, '|');

    int total = 0;
    for(int i = 0; i < size; i++) {
        printf(
            "| %03d | %-*s | %*d | %*.2f | %*.2f |\n",
            i + 1,
            NAME_LEN, list_to_prnt[i].name,
            QUANTITY_LEN, list_to_prnt[i].quantity,
            PRICE_LEN, list_to_prnt[i].price,
            PRICE_LEN, list_to_prnt[i].quantity * list_to_prnt[i].price
        );

        total += list_to_prnt[i].quantity * list_to_prnt[i].price;
    }

    prnt_line(LIST_WIDTH, '|');
    printf("|%*.2f |\n", LIST_WIDTH - 3, total);
    prnt_line(LIST_WIDTH, '+');
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

char *dup_lowered_str(char *str) {
    char *duped_str = strdup(str);
    for(int i = 0; i < strlen(duped_str); i++) duped_str[i] = tolower(duped_str[i]);
    return duped_str;
}

void prnt_line(size_t length, char side) {
    putchar(side);
    while(length-- > 2) putchar('-');
    printf("%c\n", side);
}

/**
 * @param mode  0: dau, 1: cuoi, 2: giua
 */
void prnt_text(size_t width, char *text, int mode) {
    size_t text_len = strlen(text);
    size_t pad_len = (width + text_len - 2) / 2;

    switch(mode) {
        case 0:
            printf("| %-*s |\n", width - 4, text);
            break;

        case 1:
            printf("| %*s |\n", width - 4, text);
            break;

        case 2:
            printf("|%*s%*s|\n", pad_len, text, width - pad_len - 2, " ");
            break;
    }
}

void prnt_menu() {
    prnt_line(UI_WIDTH, '+');
    prnt_text(UI_WIDTH, "MENU", 2);
    prnt_line(UI_WIDTH, '+');

    prnt_text(UI_WIDTH, "1. Them san pham", 0);
    prnt_text(UI_WIDTH, "2. Hien thi san pham theo ten tang dan", 0);
    prnt_text(UI_WIDTH, "3. Hien thi san pham theo gia tang dan", 0);
    prnt_text(UI_WIDTH, "4. Tim kiem san pham theo ten", 0);
    prnt_text(UI_WIDTH, "0. Thoat chuong trinh", 0);

    prnt_line(UI_WIDTH, '+');
}

void flush() {
    while(getchar() != '\n');
}

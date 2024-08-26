#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product{
    char name[30];
    int quantity;
    float price;
};

struct Product products[5];

void input_products();
void prnt_products();

int main() {
    input_products();
    prnt_products();

    return 0;
}

void input_products() {
    for(int i = 0; i < 5; i++) {
        printf("Nhap ten san pham %d: ", i + 1);
        scanf("%30[^\n]", products[i].name);
        
        printf("Nhap so luong san pham %d: ", i + 1);
        scanf("%d", &products[i].quantity);
        
        printf("Nhap gia san pham %d ($): ", i + 1);
        scanf("%f", &products[i].price);
        
        putchar('\n');
        while(getchar() != '\n');
    }
}

void prnt_products() {
    printf(
        "-------------------------------------------------------------------------------\n"
        "DANH MUC SAN PHAM\n"
        "-------------------------------------------------------------------------------\n"
        "|STT| %-*s | So luong | Don gia ($) | Tong tien ($) |\n"
        "-------------------------------------------------------------------------------\n",
        30, "Ten san pham"
    );
    
    float total = 0;
    for(int i = 0; i < 5; i++) {
        printf(
            "| %d | %-*s | %8d | %11.2f | %13.2f |\n",
            i + 1,
            30, products[i].name,
            products[i].quantity,
            products[i].price,
            products[i].quantity * products[i].price
        );
        total += products[i].quantity * products[i].price;
    }
    printf("-------------------------------------------------------------------------------\n");
    printf("| %75.2f |\n", total);
    printf("-------------------------------------------------------------------------------\n");
}

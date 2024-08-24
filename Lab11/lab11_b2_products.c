#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROD_NAME_LENGTH 30

struct Product{
    char name[PROD_NAME_LENGTH];
    int quantity;
    int price;
};


int main() {
    struct Product products[5];
    
    char buffer;
    for(int i = 0; i < 5; i++) {
        printf("Nhap ten san pham %d: ", i + 1);
        fgets(products[i].name, PROD_NAME_LENGTH, stdin);
        products[i].name[strcspn(products[i].name, "\n")] = '\0';
        
        printf("Nhap so luong san pham %d: ", i + 1);
        scanf("%d", &(products[i].quantity));
        
        printf("Nhap gia san pham %d: ", i + 1);
        scanf("%d", &(products[i].price));
        
        while((buffer = getchar()) != '\n' && buffer != EOF);
    }
    
    printf(
        "-------------------------------------------------------------------------------\n"
        "DANH MUC SAN PHAM\n"
        "-------------------------------------------------------------------------------\n"
        "|STT| %-*s | So luong | Don gia ($) | Tong tien ($) |\n"
        "-------------------------------------------------------------------------------\n",
        PROD_NAME_LENGTH, "Ten san pham"
    );
    
    int total = 0;
    for(int i = 0; i < 5; i++) {
        printf(
            "| %d | %-*s | %8d | %11d | %13d |\n",
            i + 1, PROD_NAME_LENGTH,
            products[i].name, products[i].quantity, products[i].price,
            products[i].quantity * products[i].price
        );
        total += products[i].quantity * products[i].price;
    }
    printf("-------------------------------------------------------------------------------\n");
    
    printf("|%76d |\n", total);
    
    printf("-------------------------------------------------------------------------------\n");
    return 0;
}
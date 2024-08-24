#include <stdio.h>

int main() {
    int input = -1;

    while(input < 0 || input > 31) {
        printf("Nhap mot so tu 0 den 31: ");
        scanf("%d", &input);
    }

    char binary[] = "00000";
    
    for(int i = 0; i < 5; i++) {
        if(input % 2 == 1) binary[4 - i] = '1';
        input /= 2;
    }

    printf("So thap phan la: %s\n", binary);

    return 0;
}
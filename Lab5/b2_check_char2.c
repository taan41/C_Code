#include <stdio.h>
#include <stdlib.h>

int main() {
    char ch;

    while(1) {
        printf("Nhap mot ky tu: ");
        ch = getchar();

        if((ch > 8 && ch < 14) || ch == 32) {
            printf("Ket thuc chuong trinh...\n");
            exit(0);
        }

        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            printf("Ky tu vua nhap la chu cai.\n");
        }
        else if(ch >= '0' && ch <= '9') {
            printf("Ky tu vua nhap la chu so.\n");
        }
        else printf("Ky tu vua nhap la ky tu dac biet.\n");

        while((ch = getchar()) != '\n' && ch != EOF);
    }

    return 0;
}
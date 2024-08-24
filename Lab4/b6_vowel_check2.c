#include <stdio.h>

int main() {
    char ch;
    
    printf("Nhap mot ky tu: ");
    scanf("%c", &ch);

    if((ch < 'A' || ch > 'Z') && (ch < 'a' || ch > 'z')) {
        printf("Ky tu vua nhap khong thuoc bang chu cai.\n");
    } else {
        printf("Ky tu vua nhap thuoc bang chu cai, va la ");

        switch(ch) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
            case 'A': case 'E': case 'I': case 'O': case 'U':
                printf("nguyen am.\n");
                break;
            default:
                printf("phu am.\n");
                break;
        }
    }

    return 0;
}
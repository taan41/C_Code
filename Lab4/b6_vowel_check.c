#include <stdio.h>

int main() {
    int input;
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};

    printf("Nhap ky tu: ");

    while((input = getchar()) == EOF) {
        printf("Invalid.\n");
    }

    if((input > 64 && input < 91) || (input > 96 && input < 123)) {
        printf("Ky tu vua nhap thuoc bang chu cai, ");
        for(int i = 0; i < sizeof(vowels); i++) {
            if(input == vowels[i]) {
                printf("va la nguyen am.\n");
                return 0;
            }
        }
        printf("va la phu am.\n");
    } else {
        printf("Ky tu vua nhap khong thuoc bang chu cai.\n");
    }

    return 0;
}
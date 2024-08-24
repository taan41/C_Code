#include <stdio.h>

int main() {
    int input;
    char vowels[] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};

    while(1) {
        printf("Nhap ky tu: ");

        while((input = getchar()) == EOF) {
            printf("Invalid.\n");
        }

        if((input > 8 && input < 14) || input == 32) {
            printf("Ket thuc chuong trinh...\n");
            return 0;
        }

        printf("Ky tu vua nhap ");
        if(input > 47 && input < 58) printf("la mot chu so.\n");
        else if((input > 64 && input < 91) || (input > 96 && input < 123)) {
            printf("thuoc bang chu cai, va ");

            int isVowel = 0;
            for(int i = 0; i < sizeof(vowels); i++) {
                if(input == vowels[i]) isVowel = 1;
            }

            printf("la %s am.\n", isVowel ? "nguyen" : "phu");
        }
        else printf("la ky tu dac biet.\n");

        while((input = getchar()) != '\n' && input != EOF);
    }
}
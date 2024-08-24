#include <stdio.h>

int main() {
    char arr[11], ch;

    printf("Nhap 10 ky tu in thuong: ");
    for(int i = 0; i < 10;) {
        ch = getchar();

        if(ch == '\n') printf("%d/10 ky tu da nhap.\n", i);
        
        // ch is not lowercase alphabet
        if(ch < 'a' || ch > 'z') continue;
        else arr[i++] = ch - 32;
    }

    arr[10] = '\0';

    printf("%s\n", arr);
}
#include <stdio.h>
#include <stdlib.h>

int main() {
    char ch, input[100];

    printf("Nhap chuoi ky tu: ");
    scanf("%99[^\n]", input);
    while((ch = getchar()) != '\n');

    printf("Nhap ky tu: ");
    ch = getchar();

    char *ptr = input;
    int ch_count = 0;
    while(*ptr) ch_count += *ptr++ == ch;

    printf("Ky tu %c xuat hien %d lan.\n", ch, ch_count);

    return 0;
}
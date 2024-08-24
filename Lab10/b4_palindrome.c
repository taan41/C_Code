#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_palindrome(char *str) {
    char *l_ptr = str, *r_ptr = str + strlen(str) - 1;
    while(l_ptr < r_ptr) if(*l_ptr++ != *r_ptr--) return 0;
    return 1;
}

int main() {
    char input[100];
    printf("Nhap chuoi ky tu: ");
    scanf("%99[^\n]", input);

    char *ptr = input;
    for(int i = 0; input[i]; i++) if(isalnum(input[i])) *ptr++ = tolower(input[i]);
    *ptr = '\0';

    printf("Chuoi %s chuoi Palindrome.\n", is_palindrome(input) ? "la" : "khong phai");
}
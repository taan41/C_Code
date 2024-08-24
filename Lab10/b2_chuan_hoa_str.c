#include <stdio.h>
#include <ctype.h>

int main() {
    char input[100];
    printf("Nhap chuoi ky tu: ");
    scanf("%99[^\n]", input);

    char *ptr = input;
    for(int i = 0; input[i]; i++) 
        if(!isspace(input[i]) || (ptr > input && !isspace(*(ptr - 1)))) 
            *ptr++ = tolower(input[i]);

    *input = toupper(*input);
    ptr -= isspace(*(ptr - 1)) ? 1 : 0;
    *ptr = '\0';

    printf("Chuoi ky tu sau khi chuan hoa:\n\"%s\"\n", input);

    return 0;
}
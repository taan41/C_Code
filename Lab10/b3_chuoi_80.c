#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void std_str(char *str) {
    char *ptr = str;
    for(int i = 0; str[i]; i++) 
        if(!isspace(str[i]) || (ptr > str && !isspace(*(ptr - 1)))) 
            *ptr++ = tolower(str[i]);

    *str = toupper(*str);
    ptr -= isspace(*(ptr - 1)) ? 1 : 0;
    *ptr = '\0';

    printf("Chuoi ky tu sau khi chuan hoa:\n\"%s\"\n", str);
}

void count_ch(char *str) {
    int ch_count = 0, vowel_count = 0, word_count = 0, is_word = 0;
    char *vowels = "aeiouAEIOU";

    for(int i = 0; str[i]; i++) {
        if(isspace(str[i])) {
            if(is_word) {
                word_count++;
                is_word = 0;
            }
        } else {
            is_word = 1;
            ch_count++;
            vowel_count += strchr(vowels, str[i]) != NULL;
        }
    }
    word_count += is_word;

    printf(
        "So ky tu trong cau: %d\n"
        "So ky tu nguyen am: %d\n"
        "Ti le nguyen am: %.2f%%\n"
        "So tu trong cau: %d\n",
        ch_count, vowel_count, (float) vowel_count / ch_count * 100, word_count
    );
}

int main() {
    char str[81];
    system("cls");
    printf("Nhap chuoi ky tu: ");
    scanf("%80[^\n]", str);

    count_ch(str);
    std_str(str);

    return 0;
}
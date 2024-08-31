#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

int main() {
    char ch;

    while(1) {
        ch = getchar();
        while(getchar() != '\n');
        printf("\033[1;1H\033[J%c\n", ch);
    }
}

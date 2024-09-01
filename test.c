#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>


int main() {
    char ch;
    // setbuf(stdin, NULL);
    setvbuf(stdin, NULL, _IONBF, BUFSIZ);
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    while((ch = getch()) != '0') putchar(ch);

}

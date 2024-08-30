#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>

int length = 10;

int main() {
    char test[100], test2[100];
    scanf("%10c|%[^\n]", test, test2);
    test[10] = '\0';
    printf("%s\n%s\n", test, test2);
}

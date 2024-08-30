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
    scanf("%[^|]|%[^\n]", test, test2);
    printf("%s\n%s\n", test, test2);
}

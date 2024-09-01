#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>


int main() {
    char str1[10], str2[20];
    memset(str2, 'a', 20);
    strcpy(str1, str2);

    printf("%s", str1);

}

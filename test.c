#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>


int main() {
    char buffer[100], str[11];
    scanf("%[^\n]", buffer);
    sscanf(buffer, "%10c:", str);
    printf("%s\n", str);

}

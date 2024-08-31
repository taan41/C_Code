#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>

typedef struct test {
    int a;
    char *b;
}test;

int main() {
    test test_list[3] = {
        {1, "01"},
        {2, "02"},
        {3, "03"}
    };

    FILE *file = fopen("test-file.dat", "wb");

    fwrite(test_list, sizeof(test), 3, file);

    fclose(file);

    file = fopen("test-file.dat", "rb");

    test *test_list_b = malloc(3 * sizeof(test));

    fread(test_list_b, sizeof(test), 3, file);

    for(int i = 0; i < 3; i++) printf("%s\n", test_list_b[i].b);

    free(test_list_b);
    fclose(file);
}

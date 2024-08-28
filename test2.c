#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

void prnt_line_1(size_t len, int double_line) {
    char ch = double_line ? '=' : '-';
    char line[len + 1];
    memset(line, ch, len);
    line[len] = '\0';
    printf("%s\n", line);
}

void prnt_line_2(size_t len, int double_line) {
    char ch = double_line ? '=' : '-';
    while(len-- > 0) putchar(ch);
    putchar('\n');
}

int main() {
    // Record the start time
    clock_t start = clock();

    // Perform the operation you want to measure
    for (int i = 0; i < 10000; i++) {
        prnt_line_1(100, 1); // Assuming prnt_line_1 is a valid function
    }

    // Record the end time
    clock_t end = clock();

    // Calculate the elapsed time in milliseconds
    double elapsed_time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

    // Print the elapsed time
    printf("Elapsed time: %.3f ms\n", elapsed_time);

    return 0;
}

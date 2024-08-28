#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>

int length = 10;

// int main() {
//     char ch, input[101];
//     int input_size = 0;

//     printf("Test: ");
//     while(1) {
//         ch = _getch();

//         if(isalnum(ch) && input_size < length) {
//             input[input_size++] = ch;
//             input[input_size] = '\0';
//             printf("\033[u\033[J\033[3%dm%s", input_size == length ? 7 : 1, input);
//         }
//         if(ch == '\b' && input_size > 0) {
//             input[--input_size] = '\0';
//             printf("\033[u\033[J\033[3%dm%s", input_size == length ? 7 : 1, input);
//         }
//         if(ch == 23) {
//             input_size = 0;
//             input[0] = '\0';
//             printf("\033[0m\033[u\033[J");
//         }

//         system("pause");
//     }

//     return 0;
// }

int main() {
    printf("Test==========================\n");
    _getch();
    printf("\033[1;1H\033[J");
    _getch();
}

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
    char test[100] = "a test string";

    printf("%s\n", test);
    
    char *ptr = strstr(test, "string");
    printf("%d\n%s\n", strlen(test) - (ptr - test), test);
    memmove(ptr + 5, ptr, 6 * sizeof(char));
    strncpy(ptr, "test ", strlen("test ") * sizeof(char));

    printf("%d\n%s\n", strlen(test) - (ptr - test), test);
}

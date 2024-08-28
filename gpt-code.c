#include <stdio.h>

int main() {
    printf("This is the first line.\n");
    printf("This is the second line.\n");
    printf("\033[A");  // Move cursor up one line
    printf("This line will overwrite the second line.\n");
    printf("\033[41mred\n\n\033[44mblue\033[0m\n");
    printf("Should del any thing behind this:0123456789\033[10D\033[K\n");

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("\033[3%d;4%dmTest %c%d\033[0m ", j, i, 'a' + i, j);
        }
        printf("\n");
    }
    return 0;
}

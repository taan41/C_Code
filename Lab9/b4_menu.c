#include <stdio.h>

void prnt_line(int length) {
    putchar('+');
    while(length-- > 2) putchar('-');
    printf("+\n");
}

void prnt_menu() {
    prnt_line(50);
    printf("%50c\r%27s\r|\n", '|', "MENU");
    prnt_line(50);

    printf(
        "%50c\r| 1. Menu 1\n"
        "%50c\r| 2. Menu 2\n"
        "%50c\r| 3. Menu 3\n"
        "%50c\r| 4. Menu 4\n"
        "%50c\r| 5. Exit\n",
        '|', '|', '|', '|', '|'
    );

    prnt_line(50);
}

int input_n() {
    int n;
    printf(" Please choose: ");
    scanf("%d", &n);

    while(n < 1 || n > 5) {
        printf(" Enter incorrectly, re-enter: ");
        scanf("%d", &n);
    };
    return n;
}

int main() {
    prnt_menu();

    int n;
    while(1) {
        n = input_n();

        if(n == 5) {
            printf(" Exit...\n");
            break;
        } else printf(" Doing menu %d...\n", n);
    }

    return 0;
}
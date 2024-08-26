#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main() {
    char ch;
    system("cls");
    printf("Press any key (ESC to exit): ");

    while((ch = _getch()) != 27) {
        system("cls");
        printf(
            "Pressed key (if displayable): %c\n"
            "(int) value of pressed key: %d\n"
            "Press any key: ",
            ch, ch
        );
    }

    printf(
        "\nESC (27) pressed\n"
        "Exiting...\n"
    );
    return 0;
}
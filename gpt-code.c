#include <stdio.h>

int main() {
    printf("This is the first line.\n");
    printf("This is the second line.\n");
    printf("\033[A");  // Move cursor up one line
    printf("This line will overwrite the second line.\n");
    printf("\033[31mThis text is red.\033[0m\n");
    return 0;
}

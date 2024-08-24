#include <stdio.h>

int main() {
    const char *options[] = {
        "CF",
        "C",
        "HDJ",
        "DreamWeaver",
        "RDBMS",
        "Learn Java By Examples"
    };

    printf("%11s\n==================\n", "MENU");

    int num_options = sizeof(options) / sizeof(options[0]);
    for(int i = 0; i < num_options; i++) {
        printf("%d. %s\n", i+1, options[i]);
    }
    
    printf("==================\nChon: ");

    int input;
    scanf("%d", &input);

    while(input < 1 || input > num_options) {
        printf("So khong hop le, xin chon lai: ");
        scanf("%d", &input);
    }
    printf("Ban chon %s!\n", options[input-1]);

    return 0;
}
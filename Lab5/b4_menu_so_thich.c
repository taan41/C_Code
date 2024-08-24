#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char *options[] = {
        "Doc sach",
        "Nghe nhac",
        "Choi the thao",
        "May tinh",
        "Thoat"
    };

    printf(" SO THICH CA NHAN\n==================\n");

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
    if(input == 5) printf("Hen gap lai!\n");
    else printf("Ban chon %s!\n", options[input-1]);

    return 0;
}
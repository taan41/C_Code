#include <stdio.h>
#include <stdlib.h>

int main() {
    printf(
        " SO THICH CA NHAN\n"
        "==================\n"
        "1. Doc sach\n"
        "2. Nghe nhac\n"
        "3. Choi the thao\n"
        "4. May tinh\n"
        "5. Thoat\n"
        "==================\n"
    );

    int input;

    while(1) {
        printf("Chon: ");
        scanf("%d", &input);

        while(input < 1 || input > 5) {
            printf("So khong hop le, hay nhap lai: ");
            scanf("%d", &input);
        }
        
        switch(input) {
            case 1:
                printf("Ban chon doc sach!\n");
                break;
            case 2:
                printf("Ban chon nghe nhac!\n");
                break;
            case 3:
                printf("Ban chon choi the thao!\n");
                break;
            case 4:
                printf("Ban chon may tinh!\n");
                break;
            default:
                printf("Hen gap lai!\n");
                exit(0);
                break;
        }
    }

    return 0;
}
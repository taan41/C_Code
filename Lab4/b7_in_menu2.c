#include <stdio.h>

int main() {
    printf(
        "%11s\n"
        "==================\n"
        "1. CF\n"
        "2. C\n"
        "3. HDJ\n"
        "4. DreamWeaver\n"
        "5. RDBMS\n"
        "6. Learn Java By Examples\n"
        "==================\n"
        "Chon: ",
        "MENU"
    );

    int input;
    scanf("%d", &input);

    while(input < 1 || input > 6) {
        printf("So vua nhap khong hop le, hay nhap lai: ");
        scanf("%d", &input);
    }

    printf("Ban chon ");
    switch(input) {
        case 1:
            printf("CF!\n");
            break;
        case 2:
            printf("C!\n");
            break;
        case 3:
            printf("HDJ!\n");
            break;
        case 4:
            printf("DreamWeaver!\n");
            break;
        case 5:
            printf("RDBMS!\n");
            break;
        case 6:
            printf("Learn Java By Examples!\n");
            break;
        default:
            break;
    }

    return 0;
}
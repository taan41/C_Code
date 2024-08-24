#include <stdio.h>

int main() {
    int a, b;
    
    printf("Nhap so thu nhat: ");
    scanf("%d", &a);
    printf("Nhap so thu hai: ");
    scanf("%d", &b);

    printf("%11s\n""==================\n""+\n""-\n""x\n"":\n""==================\n""Chon: ", "MENU");

    int input;
    scanf("%d", &input);

    while(input < 1 || input > 4) {
        printf("So khong hop le, xin chon lai: ");
        scanf("%d", &input);
    }

    switch (input)
    {
    case 1:
        printf("Tong: %d + %d = %d\n", a, b, a+b);
        break;
    
    case 2:
        printf("Hieu: %d - %d = %d\n", a, b, a-b);
        break;

    case 3:
        printf("Tich: %d x %d = %d\n", a, b, a*b);
        break;

    case 4:
        printf("Thuong: %d : %d = %.2f\n", a, b, (float) a/b);
        break;
    }

    return 0;
}
#include <stdio.h>

int main() {
    int a, b;
    
    printf("Nhap a, b: ");
    scanf("%d%d", &a, &b);

    if (a == 0) {
        if (b == 0) printf("Phuong trinh co vo so nghiem.\n");
        else printf("Phuong trinh vo nghiem.\n");
    } else {
        float x = (float) -b / a;
        printf("Nghiem: %.2f\n", x);
    }

    return 0;
}
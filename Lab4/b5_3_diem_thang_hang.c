#include <stdio.h>

int main() {
    int xA, xB, xC, yA, yB, yC;

    printf("Nhap x, y cua A: ");
    scanf("%d%d", &xA, &yA);
    printf("Nhap x, y cua B: ");
    scanf("%d%d", &xB, &yB);
    printf("Nhap x, y cua C: ");
    scanf("%d%d", &xC, &yC);

    int S = xA * (yB - yC) + xB * (yC - yA) + xC * (yA - yB);
    if(S == 0) printf("A, B, C la ba diem thang hang.\n");
    else printf("A, B, C khong phai ba diem thang hang.\n");

    return 0;
}
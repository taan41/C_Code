#include <stdio.h>

int main() {
    int a, b, *ptr_a = &a, *ptr_b = &b;
    
    printf("Nhap gia tri cua a, b: ");
    scanf("%d%d", ptr_a, ptr_b);
    
    printf(
        "Gia tri va dia chi truoc khi hoan doi:\na: %d, %p\nb: %d, %p\n",
        *ptr_a, ptr_a, *ptr_b, ptr_b
    );

    int *temp = ptr_a;
    ptr_a = ptr_b;
    ptr_b = temp;

    printf(
        "Gia tri va dia chi sau khi hoan doi:\na: %d, %p\nb: %d, %p\n",
        *ptr_a, ptr_a, *ptr_b, ptr_b
    );

    return 0;
}
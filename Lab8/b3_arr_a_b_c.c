#include <stdio.h>

int main() {
    int a[5], b[5], c[5];
    int *ptr_a = a, *ptr_b = b, *ptr_c = c;

    printf("Nhap du lieu mang a: ");
    for(int i = 0; i < 5; i++) scanf("%d", a + i);

    printf("Nhap du lieu mang b: ");
    for(int i = 0; i < 5; i++) {
        scanf("%d", b + i);
        *(c + i) = *(a + i) + *(b + i);
    }

    printf("Du lieu mang a:\n");
    for(int i = 0; i < 5; i++) printf("%d ", *(a + i));

    printf("\nDu lieu mang b:\n");
    for(int i = 0; i < 5; i++) printf("%d ", *(b + i));
    
    printf("\nDu lieu mang c:\n");
    for(int i = 0; i < 5; i++) printf("%d ", *(c + i));

    return 0;
}
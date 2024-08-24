#include <stdio.h>
#include <stdlib.h>

int main() {
    int *a;
    size_t n;

    printf("Nhap n: ");
    scanf("%u", &n);

    a = calloc(n, sizeof(int));

    printf("Nhap du lieu cua mang a: ");
    for(int i = 0; i < n; i++) scanf("%d", a + i);

    printf("Du lieu mang a:\n");
    for(int i = 0; i < n; i++) printf("%d ", *(a + i));

    free(a);
    return 0;
}
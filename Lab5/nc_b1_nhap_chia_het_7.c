#include <stdio.h>

int main() {
    int n, m;

    printf("Nhap 2 so: ");
    scanf("%d%d", &n, &m);

    if(n > m) {
        int temp = n;
        n = m;
        m = temp;
    }

    printf("Cac so chia het cho 7 tu %d den %d la:\n", n, m);

    int newline = 1;
    if(n % 7 != 0) n += 7 - n % 7;  // So chia het cho 7 sau n

    while(n < m) {
        printf("%-3d ", n);
        n += 7;

        if(newline++ % 10 == 0) printf("\n");
    }

    return 0;
}
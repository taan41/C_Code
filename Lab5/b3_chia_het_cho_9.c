#include <stdio.h>

int main() {
    printf("Cac so chia het cho 9 tu 200 den 300:\n");

    // So dau tien chia het cho 9 sau 200
    int i = 200 + (9 - 200 % 9);
    while(i < 300) {
        printf("%d ", i);
        i += 9;
    }

    return 0;
}
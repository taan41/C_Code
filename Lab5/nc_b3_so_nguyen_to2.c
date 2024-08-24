#include <stdio.h>

int main() {
    int n = 0, arr_4_primes[2] = {2, 3};

    while(n < 1) {
        printf("Nhap n > 0: ");
        scanf("%d", &n);
    }

    printf("%d so nguyen to dau tien:\n", n);
    for(int i = 0; i < 2 && i < n; i++) {
        printf("%-3d ", arr_4_primes[i]);
    }

    int line_count = 2;
    for(int i = 5; line_count < n; i+=2) {
        if(i % 2 == 0 || i % 3 == 0) continue;;

        int isPrime = 1;
        for(int j = 5; j * j <= i; j+=6) {
            if(i % j == 0 || i % (j + 2) == 0) {
                isPrime = 0;
                break;
            }
        }
        if(!isPrime) continue;

        if(line_count++ % 10 == 0) printf("\n");
        printf("%-3d ", i);
    }

    return 0;
}


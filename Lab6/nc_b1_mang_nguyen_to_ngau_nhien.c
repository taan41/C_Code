#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int isPrime(int n);

int main() {
    int arr[20], input = -1;
    while(input < 0 || input > 200) {
        printf("Nhap 1 so tu 0 den 200: ");
        scanf("%d", &input);
    }

    size_t primes_size = 1;
    int *primes = (int *)malloc(primes_size * sizeof(int));
    primes[0] = 2;
    for(int i = 3; i < 200; i+=2) {
        if(isPrime(i)) {
            primes = (int *)realloc(primes, ++primes_size * sizeof(int));
            primes[primes_size - 1] = i;
        }
    }
    
    srand(time(NULL));

    printf("Day so nguyen to ngau nhien:\n");
    unsigned int count = 0, first_pos = -1;
    for(int i = 0; i < 20; i++) {
        arr[i] = primes[rand() % primes_size];
        printf("%d ", arr[i]);

        if(input == arr[i]) {
            count++;
            if(first_pos == -1) first_pos = i;
        }
    }
    printf("\n");

    if(count == 0) printf("So vua nhap khong xuat hien.\n");
    else printf(
        "So vua nhap xuat hien %d lan.\n"
        "Vi tri xuat hien dau tien: %d\n",
        count, first_pos
        );

    return 0;
}

int isPrime(int n) {
    if(n <= 1) return 0;
    if(n <= 3) return 1;

    if(n % 2 == 0 || n % 3 == 0) return 0;

    for(int i = 5; i * i <= n; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }

    return 1;
}
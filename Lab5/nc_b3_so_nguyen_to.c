#include <stdio.h>

int main() {
    int n = 0;

    while(n < 1) {
        printf("Nhap n > 0: ");
        scanf("%d", &n);
    }

    printf("%d so nguyen to dau tien:", n);

    // mang primes[] luu cac so nguyen to da tim duoc 
    int primes[n], lastPrime = 1;

    for(int i = 0; i < n; i++) {
        // j chay tu lastPrime + 1 den khi tim dc so nguyen to moi
        for(int j = lastPrime + 1; j > 0; j++) {
            short int isPrime = 1; // 1 = true, 0 = false

            // kiem tra j co chia het cho cac phan tu cua primes[] k
            // neu chia het: j khong phai so nguyen to va huy vong lap
            for(int k = 0; k < i && primes[k] * primes[k] <= j; k++) {
                if(j % primes[k] == 0) {
                    isPrime = 0;
                    break;
                }
            }

            // j khong chia het cho primes[], j la so nguyen to
            // primes[] them j
            // lastPrime = j
            if(isPrime) {
                primes[i] = j;
                lastPrime = j;
                break;
            }
        }

        if(i % 10 == 0) printf("\n");   // In 10 so thi xuong dong
        printf("%-3d ", lastPrime);
    }

    return 0;
}
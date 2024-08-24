#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void input_long_int(long int *num_ptr);
void output_primes_to_num(long int num);
void output_line_seperator();

int main() {
    long int num = 0;
    
    printf("** Nhap -1 de ket thuc chuong trinh **\n");
    output_line_seperator();

    while (num != -1) {
        printf("Nhap 1 so lon hon 0 (hoac -1 de thoat): ");
        input_long_int(&num);
        if (num == -1) {
            output_line_seperator();
            printf("** Ket thuc chuong trinh **\n");
            break;
        }
        output_primes_to_num(num);
    }

    return 0;
}

void input_long_int(long int *num_ptr) {
    char buffer[256];
    char *end_ptr;

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin)) {
            *num_ptr = strtol(buffer, &end_ptr, 10);

            if (buffer[0] != '\n' && (*end_ptr == '\n' || *end_ptr == '\0')) {
                if (*num_ptr > 0 || *num_ptr == -1) {
                    break;
                } else {
                    printf("So vua nhap phai lon hon 0, xin nhap lai: ");
                }
            } else {
                printf("Gia tri vua nhap khong hop le, xin nhap lai: ");
            }

        } else {
            printf("Loi nhap.\n");
            while (getchar() != '\n');
        }
    }
}

void output_primes_to_num(long int num) {
    long int index = 0, *primes;
    
    printf("\nCac so nguyen to tu 0 den %ld la:\n0 1 ", num);

    if (num < 2) {
        return;
    }

    int approxPrimes = num / (log(num) - 1);
    primes = (long int*) malloc(approxPrimes * sizeof(long int));
    if (primes == NULL) {
        printf("Malloc failed.\n");
        return;
    }

    primes[index++] = 2;
    printf("2 ");

    for (long int i = 3; i <= num; i+=2) {
        int isPrime = 1;
        for (int j = 0; primes[j] * primes[j] <= i; j++) {
            if (i % primes[j] == 0) {
                isPrime = 0;
                break;
            }
        }

        if (isPrime) {
            primes[index++] = i;
            printf("%ld ", i);
            if ((index + 2) % 20 == 0) {
                printf("\n");
            }
        }
    }

    printf("\n\n");
    free(primes);
}

void output_line_seperator() {
    printf("\n-------------------------------------------\n\n");
}
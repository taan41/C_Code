#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long int reverse_n(long long int n);
char *reverse_str(const char *n_str, int n_size);

int main() {
    long long int n;
    int n_size = 0;

    printf("Nhap so chu so cua so muon dao nguoc: ");
    scanf("%d", &n_size);
    while (n_size <= 0) {
        printf("So chu so phai lon hon 0: ");
        scanf("%d", &n_size);
    }

    printf("Nhap so muon dao nguoc: ");
    scanf("%lld", &n);
    while (n < (n_size == 1 ? 0 : pow(10, n_size - 1)) || n >= (long long int) pow(10, n_size)) {
        printf("So vua nhap khong hop le: ");
        scanf("%lld", &n);
    }

    char *n_str = (char*)malloc(n_size + 1);
    sprintf(n_str, "%lld", n);

    n = reverse_n(n);
    char *reversed_n_str = reverse_str(n_str, n_size);

    printf("So dao nguoc: %lld %s\n", n, reversed_n_str);

    free(n_str);
    free(reversed_n_str);

    return 0;
}

long long int reverse_n(long long int n) {
    long long int result = 0;

    while (n != 0) {
        result = result * 10 + n % 10;
        n /= 10;
    }

    return result;
}

char *reverse_str(const char *n_str, int n_size) {
    char *result = (char*)malloc(n_size + 1);
    result[n_size] = '\0';

    for(int i = 0; i < n_size; i++) {
        result[i] = n_str[n_size - 1 - i];
    }

    return result;
}
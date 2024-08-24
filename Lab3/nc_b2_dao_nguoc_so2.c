#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *n_str = NULL;
    size_t bufsize = 0;
    size_t read;

    printf("Nhap so muon dao nguoc: ");
    read = getline(&n_str, &bufsize, stdin);

    if (read == -1) {
        printf("Error in reading");
        return 1;
    }

    int is_neg = n_str[0] == '-';
    size_t n_str_size = strlen(n_str);

    if (n_str_size > 0 && n_str[n_str_size - 1] == '\n') {
        n_str[n_str_size - 1] = '\0';
    }
    n_str_size = strlen(n_str);

    char reversed_n_str[n_str_size + 1];
    reversed_n_str[n_str_size] = '\0';
    reversed_n_str[0] = is_neg ? '-' : '\0';

    for(int i = 0 + is_neg; i < n_str_size; i++) {
        reversed_n_str[i] = n_str[n_str_size - 1 - i + is_neg];
    }

    char *n_end_ptr;
    long long int n = strtoll(n_str, &n_end_ptr, 10);
    long long int reversed_n = 0;

    while (n != 0) {
        reversed_n = reversed_n * 10 + n % 10;
        n /= 10;
    }

    printf("So dao nguoc la: %lld %s", reversed_n, reversed_n_str);

    free(n_str);

    return 0;
}

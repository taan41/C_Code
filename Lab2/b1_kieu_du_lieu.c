#include <stdio.h>

#define PRINT_SIZEOF(type) printf("%-20s: %d %s\n", #type, sizeof(type), sizeof(type) == 1 ? "byte" : "bytes")

int main() {
    printf("Kich co cua cac kieu du lieu:\n");

    PRINT_SIZEOF(_Bool);
    PRINT_SIZEOF(char);
    PRINT_SIZEOF(short);
    PRINT_SIZEOF(int);
    PRINT_SIZEOF(long);
    PRINT_SIZEOF(long long);
    PRINT_SIZEOF(float);
    PRINT_SIZEOF(double);
    PRINT_SIZEOF(long double);
    PRINT_SIZEOF(unsigned char);
    PRINT_SIZEOF(unsigned short);
    PRINT_SIZEOF(unsigned int);
    PRINT_SIZEOF(unsigned long);
    PRINT_SIZEOF(unsigned long long);

    return 0;
}
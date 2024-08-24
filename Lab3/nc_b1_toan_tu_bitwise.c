#include <stdio.h>
#include <stdlib.h>

#define BITWISE(a, b, op) bitwise(a, b, a op b, #op)

char *intToBinary(int n);
void printBinary(int n);
void bitwise(int a, int b, int result, char *op);
void bitwise_not(int n);

int main() {
    int a, b;

    printf("Nhap a va b: ");
    scanf("%d%d", &a, &b);

    printBinary(a);
    printBinary(b);

    BITWISE(a, b, &);
    BITWISE(a, b, |);
    BITWISE(a, b, ^);
    bitwise_not(a);

    return 0;
}

void printBinary(int n) {
    char *bin = intToBinary(n);
    printf("Binary cua %d = %s\n", n, bin);

    free(bin);
}

void bitwise(int a, int b, int result, char *op) {
    char *opBin = intToBinary(result);
    printf("%d %s %d = %d, binary = %s\n", a, op, b, result, opBin);

    free(opBin);
}

void bitwise_not(int n) {
    char *notBin = intToBinary(~n);
    printf("~%d = %d, binary = %s\n", n, ~n, notBin);

    free(notBin);
}

char *intToBinary(int n) {
    int bitSize = sizeof(int) * 8;
    char *binaryStr = (char*)malloc(bitSize + 1);

    if (binaryStr == NULL) {
        printf("malloc err\n");
        return NULL;
    }

    binaryStr[bitSize] = '\0';

    for (int i = 0; i < bitSize; i++) {
        binaryStr[i] = (n & (1 << (bitSize - 1 - i))) ? '1' : '0';
    }

    return binaryStr;
}
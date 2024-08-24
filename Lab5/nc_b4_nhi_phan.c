#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

int main() {
    unsigned long long int input = 0;

    while(1) {
        printf("Nhap mot so >= 0: ");
        scanf("%llu", &input);

        if(input == 10) break;

        size_t size;
        if(input <= UINT8_MAX) size = sizeof(uint8_t) * 8;
        else if (input <= UINT16_MAX) size = sizeof(uint16_t) * 8;
        else if (input <= UINT32_MAX) size = sizeof(uint32_t) * 8;
        else size = sizeof(uint64_t) * 8; 
        // size = log2l(input) == 0 ? 1 : (log2l(input) + 1);
        char binary[size+1];
        binary[size] = '\0';

        for(int i = size - 1; i >= 0; i--) {
            binary[i] = (input % 2 == 0) ? '0' : '1';
            input /= 2;
        }

        printf("%s\n", binary);
    }

    return 0;
}
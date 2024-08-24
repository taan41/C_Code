#include <stdio.h>
#include <stdint.h>

#define GET_SIZE(bit) else if(input <= UINT##bit##_MAX) {size = sizeof(uint##bit##_t) * 8;}

int main() {
    uint64_t input;

    while(1) {
        printf("Nhap mot so bat ky: ");
        scanf("%llu", &input);
        if(input == 69) break;

        size_t size;
        if(0) {}
        GET_SIZE(8)
        GET_SIZE(16)
        GET_SIZE(32)
        GET_SIZE(64)
        char binary[size + 1];
        binary[size] = '\0';

        for(int i = size - 1; i >= 0; i--) {
            binary[i] = (input & 1 << (size - 1 - i)) ? '1' : '0';
        }

        printf("%s\n", binary);
    }

    return 0;
}
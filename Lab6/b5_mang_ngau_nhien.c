#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int arr[10], input = -1;
    while(input < 0 || input > 100) {
        printf("Nhap 1 so tu 0 den 100: ");
        scanf("%d", &input);
    }
    
    srand(time(NULL));

    printf("Day so ngau nhien:\n");
    int count = 0, first_pos = -1;
    for(int i = 0; i < 10; i++) {
        arr[i] = rand() % 101;
        printf("%d ", arr[i]);

        if(input == arr[i]) {
            count++;
            if(first_pos == -1) first_pos = i;
        }
    }
    printf("\n");

    if(!count) printf("So vua nhap khong xuat hien.\n");
    else printf(
        "So vua nhap xuat hien %d lan.\n"
        "Vi tri xuat hien dau tien: %d\n",
        count, first_pos
        );

    return 0;
}
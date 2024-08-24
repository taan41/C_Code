#include <stdio.h>
#include <stdlib.h>

void input_long_int(long int *num_ptr);
void output_even_num(long int num);
void output_line_seperator();

int main() {
    long int num = 0L;

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
        output_even_num(num);
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

void output_even_num(long int num) {
    printf("\nCac so chan tu 0 den %ld:\n", num);

    for (long int i = 0; i <= num; i+=2) {
        printf("%ld ", i);
        if (i > 0 && i % 50 == 0) {
            printf("\n");
        }
    }

    printf("\n\n");
}

void output_line_seperator() {
    printf("\n-------------------------------------------\n\n");
}
#include <stdio.h>

void scan_double(char *num_name, double *num_ptr);
void print_double(char *pre_msg, double num, char *sub_msg);

int main() {
    double a, b;

    scan_double("a", &a);
    scan_double("b", &b);

    print_double("a + b = ", a + b, "");
    print_double("a - b = ", a - b, "");
    print_double("a * b = ", a * b, "");

    if (b == 0) {
        printf("Khong the chia cho 0\n");
    } else {
        print_double("a / b = ", a / b, "");
    }

    return 0;
}

void scan_double(char *num_name, double *num_ptr) {
    printf("Nhap %s: ", num_name);
    scanf("%lf", num_ptr);
}

void print_double(char *pre_msg, double num, char *sub_msg) {
    int precision = (num == (int) num) ? 0 : 2;
    printf("%s%.*lf%s\n", pre_msg, precision, num, sub_msg);
}
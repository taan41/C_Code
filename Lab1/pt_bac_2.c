#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void input_num(const char *num_name, double *num);
void output_result(const int result_num, double *results);

int main() {
    double a, b, c;
    double delta, x[2];

    printf("Phuong trinh co dang ax^2 + bx + c = 0\n");

    input_num("a", &a);
    input_num("b", &b);
    input_num("c", &c);

    if (a == 0) {
        if (b == 0) {
            output_result(0, NULL);
        } else {
            x[0] = -c / b;
            output_result(1, x);
        }
    } else {
        delta = b * b - 4 * a * c;
        if (delta > 0) {
            x[0] = (-b + sqrt(delta)) / (2 * a);
            x[1] = (-b - sqrt(delta)) / (2 * a);
            output_result(2, x);
        } else if (delta == 0) {
            x[0] = -b / (2 * a);
            output_result(1, x);
        } else {
            output_result(0, NULL);
        }
    }

    return 0;
}

void input_num(const char *num_name, double *num) {
    printf("Nhap %s: ", num_name);

    char buffer[256];
    char *end_ptr;
    while (1) {
        if(fgets(buffer, sizeof(buffer), stdin)) {
            *num = strtod(buffer, &end_ptr);
            if (buffer[0] != '\n' && (*end_ptr == '\0' || *end_ptr == '\n')) {
                break;
            } else {
                printf("Gia tri vua nhap khong hop le, xin nhap lai %s: ", num_name);
            }
        } else {
            printf("Loi nhap.\n");
            while (getchar() != '\n');
        }
    }
}

void output_result(const int result_num, double *results) {
    if (result_num == 0 || result_num < 0) {
        printf("Phuong trinh vo nghiem.");
    } else {
        printf("Phuong trinh co %d nghiem:\n", result_num);
        for (int i = 0; i < result_num; i++) {
            if (results[i] == 0) {
                printf("0\n");
            } else if ((int) results[i] == results[i]) {
                printf("%.0lf\n", results[i]);
            } else {
                printf("%.2lf\n", results[i]);
            }
        }
    }
}
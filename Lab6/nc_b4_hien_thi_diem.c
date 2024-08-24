#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    int mode = 0;
    while(mode != 1 && mode != 2) {
        printf("Chon che do nhap (1 = thu cong, 2 = ngau nhien): ");
        scanf("%d", &mode);
    }

    float arr_diem[5][7] = {{0}};
    if(mode == 1) {
        for(int i = 0; i < 5; i++) {
            printf("Nhap 6 diem cho HS %d: ", i+1);

            float avg = 0;
            for(int j = 0; j < 6; j++) {
                scanf("%f", &arr_diem[i][j]);
                avg += arr_diem[i][j] / 6;
            }
            arr_diem[i][6] = avg;

            char flush;
            while((flush = getchar()) != '\n' && flush != EOF);
        }
    } else {
        srand(time(NULL));

        for(int i = 0; i < 5; i++) {
            float avg = 0;

            for(int j = 0; j < 6; j++) {
                arr_diem[i][j] = rand() % 101 + rand() % 2 * 0.5;
                if(arr_diem[i][j] > 100) arr_diem[i][j] = 100;
                avg += arr_diem[i][j] / 6;
            }

            arr_diem[i][6] = avg;
        }
    }

    printf(
        "Hoc vien | %5s | %5s | %5s | %5s | %5s | %5s | %5s |\n"
        "------------------------------------------------------------------\n",
        "CF", "C", "HDJ", "DW", "RDBMS", "Java", "TB"
    );

    for(int i = 0; i < 5; i++) {
        printf("HV %-5d |", i+1);
        for(int j = 0; j < 7; j++) {
            printf(" %5.1f |", arr_diem[i][j]);
        }
        printf("\n");
    }
}
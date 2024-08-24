#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Mark{
    char subject[80];
    float written;
    float practical;
}Mark;


int main() {
    struct Mark *marks;
    size_t sz_marks = 0;
    
    printf(
        "1. Nhap diem\n"
        "2. Xem diem\n"
        "0. Thoat\n"
    );
    
    int input = 0;
    char buffer_clear;
    for(;;) {
        printf("Moi ban nhap: ");
        scanf("%d", &input);
        
        while((buffer_clear = getchar()) != '\n' && buffer_clear != EOF);
        
        switch(input) {
            case 1:
                if(sz_marks++ == 0) marks = malloc(sizeof(Mark));
                else marks = realloc(marks, sz_marks * sizeof(Mark));
                
                printf("Nhap ten mon hoc: ");
                fgets(marks[sz_marks - 1].subject, 80, stdin);
                marks[sz_marks - 1].subject[strcspn(marks[sz_marks - 1].subject, "\n")] = '\0';
                printf("Nhap diem ly thuyet: ");
                scanf("%f", &(marks[sz_marks - 1].written));
                printf("Nhap diem thuc hanh: ");
                scanf("%f", &(marks[sz_marks - 1].practical));
                
                break;
                
            case 2:
                printf(
                    "THONG TIN VE DIEM\n"
                    "--------------------------\n"
                );
                
                for(int i = 0; i < sz_marks; i++) {
                    printf(
                        "Mon hoc: %s\n"
                        "Diem LT: %.2f\n"
                        "Diem TH: %.2f\n"
                        "Diem TB: %.2f\n"
                        , marks[i].subject, marks[i].written, marks[i].practical,
                        (marks[i].written + marks[i].practical) / 2);
                }
                printf("--------------------------\n");
                
                break;
                
            case 0:
                exit(0);
                break;
                
            default:
                break;
        }
    }
    return 0;
}
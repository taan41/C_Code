#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Mark{
    char subject[80];
    float written;
    float practical;
}Mark;

void input_mark(Mark *mark);
void prnt_marks(Mark *mark);

int main() {
    Mark mark;

    input_mark(&mark);
    prnt_marks(&mark);

    return 0;
}

void input_mark(Mark *mark) {
    printf("Nhap ten mon hoc: ");
    scanf("%80[^\n]", mark->subject);
    printf("Nhap diem ly thuyet: ");
    scanf("%f", &mark->written);
    printf("Nhap diem thuc hanh: ");
    scanf("%f", &mark->practical);
}

void prnt_marks(Mark *mark) {
    printf(
        "--------------------------\n"
        "THONG TIN VE DIEM\n"
        "--------------------------\n"
    );
    
    printf(
        "Mon hoc: %s\n"
        "Diem LT: %.2f\n"
        "Diem TH: %.2f\n"
        "Diem TB: %.2f\n",
        mark->subject,
        mark->written,
        mark->practical,
        (mark->written + mark->practical) / 2
    );
    printf("--------------------------\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_WIDTH 50

typedef struct Subject {
    char name[80];
    float written;
    float practical;
    float average;
}Subject;

void add_subj(Subject *subj);

void prnt_line(size_t length, int dbl_line);
void prnt_text(size_t width, char *text, int mode);
void prnt_menu();
void flush();

int main() {
    struct Subject subjects[6];
    prnt_menu();
    
    int input = 0, index = 0, max_avg_i = 0;
    for(;;) {
        printf("Moi ban nhap lenh menu: ");
        scanf("%d", &input);
        flush();
        system("cls");
        prnt_menu();
        
        switch(input) {
            case 1:
                if(index == 6) {
                    printf("Da het sanh sach\n");
                    break;
                }
                
                printf("Nhap ten mon hoc: ");
                scanf("%80s", &(subjects[index].name));
                flush();
                
                printf("Nhap diem ly thuyet: ");
                scanf("%f", &(subjects[index].written));
                printf("Nhap diem thuc hanh: ");
                scanf("%f", &(subjects[index].practical));

                subjects[index].average = (subjects[index].written + subjects[index].practical) / 2;
                if(subjects[index].average > subjects[max_avg_i].average) max_avg_i = index;
                
                index++;
                break;
            
            case 2:
                printf("THONG TIN VE DIEM\n");
                prnt_line(MENU_WIDTH, 0);
                
                for(int i = 0; i < index; i++) {
                    printf(
                        "Mon hoc: %s\n"
                        "Diem LT: %.2f\n"
                        "Diem TH: %.2f\n"
                        "Diem TB: %.2f\n\n",
                        subjects[i].name, subjects[i].written,
                        subjects[i].practical, subjects[i].average
                    );
                }
                break;

            case 3:
                printf(
                    "Mon hoc co diem TB cao nhat:\n"
                    "Mon hoc: %s\n"
                    "Diem LT: %.2f\n"
                    "Diem TH: %.2f\n"
                    "Diem TB: %.2f\n",
                    subjects[max_avg_i].name, subjects[max_avg_i].written,
                    subjects[max_avg_i].practical, subjects[max_avg_i].average
                );
                break;
            
            case 0:
                printf("Ket thuc chuong trinh...\n");
                exit(0);
                break;
            
            default:
                printf("Nhap lai.\n");
                break;
        }
        prnt_line(MENU_WIDTH, 0);
    }
                
    return 0;
}


void prnt_line(size_t length, int dbl_line) {
    char line = '-';
    if(dbl_line) line = '='; 
    while(length-- > 0) printf("%c", line);
    printf("\n");
}

void prnt_text(size_t width, char *text, int mode) {
    size_t text_length = strlen(text);
    size_t pad_length = (width - text_length - 2) / 2;

    switch(mode) {
        case 0:
            printf("| %-*s |\n", width - 4, text);
            break;

        case 1:
            printf("| %*s |\n", width - 4, text);
            break;

        case 2:
            printf("|%*s%s%*s|\n", pad_length, " ", text, width - pad_length - text_length - 2, " ");
            break;
    }
}

void prnt_menu() {
    prnt_line(MENU_WIDTH, 1);
    prnt_text(MENU_WIDTH, "MENU", 2);
    prnt_line(MENU_WIDTH, 0);
    
    prnt_text(MENU_WIDTH, " 1. Nhap diem", 0);
    prnt_text(MENU_WIDTH, " 2. Hien thi danh sach", 0);
    prnt_text(MENU_WIDTH, " 3. Diem TB cao nhat", 0);
    prnt_text(MENU_WIDTH, " 0. Thoat chuong trinh", 0);
    
    prnt_line(MENU_WIDTH, 1);
}

void flush() {
    char buffer;
    while((buffer = getchar()) != '\n' && buffer != EOF);
}

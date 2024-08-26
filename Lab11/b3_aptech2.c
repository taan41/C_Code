#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MENU_WIDTH 50

typedef struct Subject {
    char name[80];
    float written;
    float practical;
    float average;
}Subject;

void input_subj();
void prnt_subj(int index);
void prnt_subjs();
void prnt_max_avg();

void prnt_line(int dbl_line);
void prnt_text(char *text, int mode);
void prnt_menu();
void flush();

struct Subject subjs[6];
static int subjs_size = 0, max_avg = 0;

int main() {
    int choice;

    while(1) {
        system("cls");
        prnt_menu();
        printf("Nhap lenh menu: ");
        scanf("%d", &choice);
        flush();
        
        switch(choice) {
            case 1: input_subj(); break;
            
            case 2: prnt_subjs(); break;

            case 3: prnt_max_avg(); break;
            
            case 0:
                printf("Ket thuc chuong trinh...\n");
                exit(0);
                break;
            
            default:
                printf("Nhap lai\n");
                break;
        }

        prnt_line(0);
        printf("Nhan phim bat ky de tiep tuc...");
        _getch();
    }
                
    return 0;
}

void input_subj() {
    if(subjs_size >= 6) {
        printf("Da het sanh sach\n");
        return;
    }
    
    printf("Nhap ten mon hoc: ");
    scanf("%80[^\n]", subjs[subjs_size].name);
    flush();
    
    printf("Nhap diem ly thuyet: ");
    scanf("%f", &subjs[subjs_size].written);

    printf("Nhap diem thuc hanh: ");
    scanf("%f", &subjs[subjs_size].practical);

    subjs[subjs_size].average = (subjs[subjs_size].written + subjs[subjs_size].practical) / 2;
    if(subjs[subjs_size].average > max_avg) max_avg = subjs[subjs_size].average;
    
    subjs_size++;
}

void prnt_subjs() {
    if(subjs_size == 0) {
        printf("Chua co mon hoc trong danh sach\n");
        return;
    }

    prnt_line(1);
    printf("THONG TIN VE DIEM\n");

    prnt_line(0);
    for(int i = 0; i < subjs_size; i++) {
        prnt_subj(i);
    }
}

void prnt_max_avg() {
    if(subjs_size == 0) {
        printf("Chua co mon hoc trong danh sach\n");
        return;
    }

    printf("Cac mon hoc co diem TB cao nhat:\n");
    prnt_line(0);

    for(int i = 0; i < subjs_size; i++) {
        if(subjs[i].average == max_avg) prnt_subj(i);
    }
}

void prnt_subj(int index) {
    printf(
        "Mon hoc: %s\n"
        "Diem LT: %.2f\n"
        "Diem TH: %.2f\n"
        "Diem TB: %.2f\n\n",
        subjs[index].name,
        subjs[index].written,
        subjs[index].practical,
        subjs[index].average
    );

}

void prnt_line(int dbl_line) {
    char ch = '-';
    if(dbl_line) ch = '=';
    for(int i = 0; i < MENU_WIDTH; i++) putchar(ch);
    putchar('\n');
}

/**
 * @param mode  0: in dau dong, 1: in cuoi dong, 2: in giua dong
 */
void prnt_text(char *text, int mode)
{
    size_t text_len = strlen(text);
    size_t pad_len = (MENU_WIDTH + text_len - 2) / 2;

    switch(mode) {
        case 0:
            printf("| %-*s |\n", MENU_WIDTH - 4, text);
            break;

        case 1:
            printf("| %*s |\n", MENU_WIDTH - 4, text);
            break;

        case 2:
            printf("|%*s%*s|\n", pad_len, text, MENU_WIDTH - pad_len - 2, "");
            break;
    }
}

void prnt_menu() {
    prnt_line(1);
    prnt_text("MENU", 2);
    prnt_line(0);
    
    prnt_text(" 1. Nhap diem", 0);
    prnt_text(" 2. Hien thi danh sach", 0);
    prnt_text(" 3. Diem TB cao nhat", 0);
    prnt_text(" 0. Thoat chuong trinh", 0);
    
    prnt_line(1);
}

void flush() { while(getchar() != '\n'); }

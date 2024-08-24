#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

#define MENU_WIDTH 60

#define STUDENT_NAME_LEN 50

void students_add();
void students_find();
void students_std();
void students_print();

void prnt_line(size_t len);
void prnt_menu();
void flush();

char **STUDENTS;
size_t STD_SIZE = 0, BUFFER_SIZE = 10;

int main() {
    STUDENTS = malloc(BUFFER_SIZE * sizeof(char *));

    int menu_choice;

    system("cls");
    prnt_menu();

    while(1) {
        scanf("%d", &menu_choice);
        flush();

        switch(menu_choice) {
            case 1:
                students_add();
                goto return_menu_label;

            case 2:
                students_find();
                goto return_menu_label;

            case 3:
                students_std();
                goto return_menu_label;

            case 4:
                students_print();
                goto return_menu_label;

            return_menu_label:
                printf("Nhan phim bat ky de tiep tuc...");
                _getch();
                system("cls");
                prnt_menu();
                break;

            case 0:
                printf("Thoat chuong trinh...\n");
                exit(0);
                break;

            default:
                printf("Khong hop le, vui long nhap lai: ");
                break;
        }
    }
}

void students_add() {
    if(STD_SIZE >= BUFFER_SIZE) {
        BUFFER_SIZE *= 2;
        STUDENTS = realloc(STUDENTS, BUFFER_SIZE * sizeof(char *));
    }

    int add_amount;
    printf("Nhap so hoc vien them vao: ");
    scanf("%d", &add_amount);
    flush();

    while(add_amount-- > 0) {
        STUDENTS[STD_SIZE] = malloc((STUDENT_NAME_LEN + 1) * sizeof(char));
        printf("Nhap ten hoc vien: ");
        fgets(STUDENTS[STD_SIZE], STUDENT_NAME_LEN, stdin);

        if(strchr(STUDENTS[STD_SIZE], '\n')) *strchr(STUDENTS[STD_SIZE], '\n') = 0;
        else STUDENTS[STD_SIZE][STUDENT_NAME_LEN] = 0;

        printf("Da them %s!\n", STUDENTS[STD_SIZE]);
        STD_SIZE++;
    }

    prnt_line(MENU_WIDTH);
}

char *lower_str(char *str) {
    char *result = strdup(str);
    for(int i = 0; result[i]; i++) result[i] = tolower(result[i]);
    return result;
}

void students_find() {
    char input[STUDENT_NAME_LEN + 1];
    printf("Nhap ten can tim: ");
    fgets(input, STUDENT_NAME_LEN, stdin);

    if(strchr(input, '\n')) *strchr(input, '\n') = 0;
    else input[STUDENT_NAME_LEN] = 0;

    printf("Ket qua tim kiem:\n");
    int count = 0;
    for(int i = 0; i < STD_SIZE; i++) {
        if(strstr(lower_str(STUDENTS[i]), lower_str(input)))
            printf(" %d. %s\n", ++count, STUDENTS[i]);
    }
    if(!count) printf("Khong tim thay ten hoc vien nao chua '%s'.\n", input);

    prnt_line(MENU_WIDTH);
}

void standardize_str(char *str) {
    char *ptr = str;
    int is_word = 0;
    for(int i = 0; str[i]; i++) {
        if(isalpha(str[i])) {
            *ptr++ = is_word ? tolower(str[i]) : toupper(str[i]);
            is_word = 1;
        }
        else if (isspace(str[i])) {
            if(is_word) *ptr++ = ' ';
            is_word = 0;
        }
    }

    *str = toupper(*str);
    ptr -= !is_word;
    *ptr = '\0';
}

void students_std() {
    int width = 13 + STUDENT_NAME_LEN * 2;
    int pad1 = (STUDENT_NAME_LEN + 6) / 2;
    int pad2 = (STUDENT_NAME_LEN + 17) / 2;

    prnt_line(width);
    printf(
        "| STT | %*s%*s | %*s%*s |\n",
        pad1, "TEN CU", STUDENT_NAME_LEN - pad1, "",
        pad2, "TEN SAU CHUAN HOA", STUDENT_NAME_LEN - pad2, ""
    );
    prnt_line(width);

    for(int i = 0; i < STD_SIZE; i++) {
        printf("| %03d | %-*s | ", i + 1, STUDENT_NAME_LEN, STUDENTS[i]);
        standardize_str(STUDENTS[i]);
        printf("%-*s |\n", STUDENT_NAME_LEN, STUDENTS[i]);
    }

    prnt_line(width);
}

void students_print() {
    int width = 10 + STUDENT_NAME_LEN;
    int pad = (STUDENT_NAME_LEN + 12) / 2;

    prnt_line(width);
    printf("| STT | %*s%*s |\n", pad, "TEN HOC VIEN", STUDENT_NAME_LEN - pad, "");
    prnt_line(width);

    for(int i = 0; i < STD_SIZE; i++) {
        printf("| %03d | %-*s |\n", i + 1, STUDENT_NAME_LEN, STUDENTS[i]);
    }

    prnt_line(width);
}

void prnt_line(size_t len) {
    while(len-- > 0) putchar('-');
    putchar('\n');
}

void prnt_menu() {
    prnt_line(MENU_WIDTH);
    printf("%*s\n", (MENU_WIDTH + 4) / 2, "MENU");
    prnt_line(MENU_WIDTH);

    printf(
        " 1. Nhap them mot hoc vien moi\n"
        " 2. Tim kiem hoc vien\n"
        " 3. Chuan hoa ten hoc vien\n"
        " 4. Hien thi danh sach hoc vien\n"
        " 0. Thoat chuong trinh\n"
    );

    prnt_line(MENU_WIDTH);
    printf("Moi ban chon: ");
}

void flush() {
    static char ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

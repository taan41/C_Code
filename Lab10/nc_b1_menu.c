#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SNAME_MAX_LENGTH 40
#define MENU_WIDTH 50
#define SLIST_WIDTH 5 + 5 + SNAME_MAX_LENGTH
#define STDLIST_WIDTH 5 + 8 + 2 * SNAME_MAX_LENGTH

void add_students(char ***students, size_t *sz_students);
void find_students(char ***students, size_t *sz_students);
void standardize_students(char ***students, size_t *sz_students);
void prnt_students(char ***students, size_t *sz_students);

void prnt_line(int length);
void prnt_menu();

const char *options[] = {
    "Thoat chuong trinh",
    "Nhap them mot hoc vien moi",
    "Tim kiem hoc vien",
    "Chuan hoa ten hoc vien",
    "Hien thi danh sach hoc vien",
};
const size_t sz_opt = sizeof(options) / sizeof(options[0]);

int main() {;
    prnt_menu();
    
    int input = 0;
    char buffer_clear;
    char **students;
    size_t sz_students = 0;
    for(;;) {
        printf("Moi ban chon: ");
        scanf("%d", &input);
        while((buffer_clear = getchar()) != '\n' && buffer_clear != EOF);
        if(input > 0 && input < sz_opt) printf("%s\n", options[input]);
        
      
        switch(input) {
            case 1:
                add_students(&students, &sz_students);
                break;
            
            case 2:
                find_students(&students, &sz_students);
                break;
            
            case 3:
                standardize_students(&students, &sz_students);
                break;
            
            case 4:
                prnt_students(&students, &sz_students);
                break;
            
            case 0:
                for(int i = 0; i < sz_students; i++) free(students[i]);
                free(students);
                printf("Ket thuc chuong trinh...");
                exit(0);
                break;
                
            default:
                printf("Khong hop le, hay nhap lai.\n");
                break;
        }
    }
}

void add_students(char ***students, size_t *sz_students) {
    char student_name[SNAME_MAX_LENGTH];
    printf("Nhap ten hoc vien: ");
     fgets(student_name, SNAME_MAX_LENGTH, stdin);
     student_name[strcspn(student_name, "\n")] = '\0';
     
     (*sz_students)++;
     if(*sz_students == 1) *students = (char **)malloc(*sz_students * sizeof(char *));
     else *students = (char**)realloc(*students, *sz_students * sizeof(char *));
     
     (*students)[*sz_students - 1]  = strdup(student_name);
}

char *to_lower(char *str) {
    char *str_lower = strdup(str);
    for(int i = 0; i < strlen(str_lower); i++) str_lower[i] = tolower(str_lower[i]);
    return str_lower;
}

void find_students(char ***students, size_t *sz_students) {
    char name_to_find[SNAME_MAX_LENGTH];
    printf("Nhap ten can tim: ");
    fgets(name_to_find, SNAME_MAX_LENGTH, stdin);
    name_to_find[strcspn(name_to_find, "\n")] = '\0';
    
    for(int i = 0; i < *sz_students; i++) {
        if(strstr(to_lower((*students)[i]), to_lower(name_to_find)) != NULL) printf("%s\n", (*students)[i]);
    }
};

void standardize_str(char **str) {
    int is_word = 0;
    int std_i = 0;

    for(int i = 0; i < strlen(*str); i++) {
        if(isspace((*str)[i])) {
            if(is_word) {
                (*str)[std_i++] = ' ';
                is_word = 0;
            }
        } else {
            if(is_word) {
                if(isupper((*str)[i])) (*str)[i] += 32;
            }
            else {
                is_word = 1;
                if(islower((*str)[i])) (*str)[i] -= 32;
            }
            (*str)[std_i++] = (*str)[i];
        }
    }

    (*str)[std_i] = ' ';
    while(isspace((*str)[std_i])) std_i--;
    (*str)[std_i + 1] = '\0';
    
}

void standardize_students(char ***students, size_t *sz_students) {
    prnt_line(STDLIST_WIDTH);
    
    char header1[] = "TEN CU";
    char header2[] = "TEN SAU CHUAN HOA";
    int pad1 = (SNAME_MAX_LENGTH + 2 - strlen(header1)) / 2 + strlen(header1);
    int pad2 = (SNAME_MAX_LENGTH + 2 - strlen(header2)) / 2 + strlen(header2);
    printf("| STT |%*s%*s|%*s%*s|\n", pad1, header1, SNAME_MAX_LENGTH + 2 - pad1, " ", pad2, header2, SNAME_MAX_LENGTH + 2 - pad2, " ");
    
    prnt_line(STDLIST_WIDTH);
    for(int i = 0; i < *sz_students; i++) {
        printf("| %03d | %-*s |", i + 1, SNAME_MAX_LENGTH, (*students)[i]);
        standardize_str(&(*students)[i]);
        printf(" %-*s |\n", SNAME_MAX_LENGTH, (*students)[i]);
    }
    
    prnt_line(STDLIST_WIDTH);
};

void prnt_students(char ***students, size_t *sz_students) {
    prnt_line(SLIST_WIDTH);
    
    char header[] = "TEN HOC VIEN";
    int pad = (SNAME_MAX_LENGTH + 2 - strlen(header)) / 2 + strlen(header);
    printf("| STT |%*s%*s|\n", pad, header, SNAME_MAX_LENGTH + 2 - pad, " ");
    
    prnt_line(SLIST_WIDTH);
    
    for(int i = 0; i < *sz_students; i++) {
        printf("| %03d | %-*s |\n", i + 1, SNAME_MAX_LENGTH, (*students)[i]);
    }
    
    prnt_line(SLIST_WIDTH);
}

void prnt_line(int length) {
    printf("+");
    for(int i = 2; i < length; i++) printf("-");
    printf("+\n");
}

void prnt_menu() {
    prnt_line(MENU_WIDTH);
    
    printf("|%22s%s%22s|\n", " ", "MENU", " ");
    prnt_line(MENU_WIDTH);
    
    for(int i = 1; i < sz_opt; i++) printf("| %d. %-44s|\n", i, options[i]);
    printf("| 0. %-44s|\n", options[0]);
    
    prnt_line(MENU_WIDTH);
}
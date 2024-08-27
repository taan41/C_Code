#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include "extrastuffs.c"

#define UI_WIDTH 50
#define UI_PROMPT_MSG_LENGTH 20
#define UI_NOTI_MSG_LENGTH 50

#define ATM_META_LEN 2
#define ATM_NAME_LEN 50
#define ATM_ACCOUNT_LEN 14
#define ATM_PIN_LEN 6
#define ATM_BALANCE_LEN 18
#define ATM_BALANCE_MIN_VALUE 50000
#define ATM_DATA_LEN ATM_NAME_LEN + ATM_ACCOUNT_LEN + ATM_PIN_LEN + ATM_BALANCE_LEN

const char *BANK_NAME = "VTC Academy Bank";

typedef struct ATM
{
    char name[ATM_NAME_LEN + 1];
    char account[ATM_ACCOUNT_LEN + 1];
    char pin[ATM_PIN_LEN + 1];
    long long int balance;
} ATM;


void atm_input(ATM *atm);
void atm_save(ATM *atm);
void atm_randomize(ATM *atm);

int unbuffered_input(char *target, size_t max_len, int mode, int is_censored);
void prnt_line(size_t len, char ch);
void prnt_menu();
void flush();

ATM GLB_ATM;

int main() {
    char test_buffer[11] = "testbuffer";
    int testing = 1;
    if(testing) goto test_label;

    ATM atm = {
        .name = "default name",
        .pin = "000000",
        .balance = 50000
    };
    srand(time(NULL));
    char ch;

    while(1) {
        atm_randomize(&atm);
        prnt_menu();

        atm_input(&atm);

        prnt_line(UI_WIDTH, '-');
        printf(
            " Successfully entered account info!\n"
            " Do you want to create ATM Card (save to file)? (Y/N): "
        );
        if((ch = getchar()) == 'Y' || ch == 'y') atm_save(&atm);
        flush();

        prnt_line(UI_WIDTH, '-');
        printf(" Do you want to add a new account? (Y/N): ");
        if((ch = getchar()) != 'Y' && ch != 'y') exit(0);
        flush();
    }


    test_label:
    if(!testing) goto done_testing_label;

    printf("Test unbuffered input: ");
    unbuffered_input(test_buffer, 10, 3, 0);
    printf("\nInput: %s\n", test_buffer);

    done_testing_label:

    return 0;
}

/**
 * @brief   Doc du lieu nhap vao tu nguoi dung va chuyen doi sang thong tin tai khoan atm
 * 
 * @param target        Con tro den bien luu du lieu, co the la char* hoac long long int*
 * @param length        Do dai toi da cua du lieu
 * @param list_compare  Danh sach kiem tra account no trung lap, NULL neu khong phai account no
 * @param list_size
 * @param digit_only    Neu = 1 chi chap nhan 0-9, neu = 0 chi chap nhan A-z va ' '
 * @param return_lld    Neu = 1 luu du lieu duoi dang long long int, neu = 0 luu dang char*
 * @param exact_length  Neu = 1 du lieu phai co do dai bang 'length', neu = 0 co the ngan hon 'lenght'
 * @param min_value     Neu 'return_lld' = 1 du lieu phai lon hon hoac bang 'min_value'
 * @param ui_prompt_msg Thong diep hien thi truoc khi nguoi dung nhap du lieu
 */
void atm_input_info(void *target, size_t len, char **list_compare, size_t list_size, int digit_only, int return_lld, int exact_length, long long int min_value, char *ui_prompt_msg) {
    size_t input_len = 1;
    size_t buffer_size = 10;
    char ch, *input = malloc(buffer_size * sizeof(char));
    input[0] = '\0';

    printf(" %-*s", UI_PROMPT_MSG_LENGTH, ui_prompt_msg);
    ch = _getch();

    while(1) {
        if((digit_only ? isdigit(ch) : (isalpha(ch) || ch == ' ')) && input_len < len + 1) {
            if(input_len >= buffer_size) {
                buffer_size *= 2;
                input = realloc(input, buffer_size * sizeof(char));
            }
            input[input_len - 1] = ch;
            input[input_len++] = '\0';

            putchar(ch);
        }
        else if(ch == '\n' || ch == '\r') {
            int valid = exact_length ? input_len == len + 1 : (input_len > 1 && input_len <= len + 1);
            if(return_lld) valid = strtoll(input, NULL, 10) >= min_value;

            int duped = 0;
            if(list_compare != NULL) {
                for(int i = 0; !duped && i < list_size; i++) duped = strcmp(list_compare[i], input) == 0;
            }

            if(valid && !duped) break;

            printf(
                "\r %*s\r %-*s%s%s",
                UI_PROMPT_MSG_LENGTH + input_len, "",
                UI_PROMPT_MSG_LENGTH, ui_prompt_msg,
                duped ? "Duplicated" : "Invalid",
                ", please re-enter"
            );

            input_len = 1;
            input[0] = '\0';

            ch = _getch();
            printf(
                "\r %*s\r %-*s",
                UI_PROMPT_MSG_LENGTH  + UI_NOTI_MSG_LENGTH, "",
                UI_PROMPT_MSG_LENGTH, ui_prompt_msg
            );
            continue;
        }
        else if(ch == '\b' && input_len > 1) {
            printf("\b \b");
            input[--input_len - 1] = '\0';
        }
        else if(ch == 27) {
            if(return_lld) printf("%lld\n", *(long long int *)target);
            else printf("%s\n", target);

            return;
        }

        ch = _getch();
    }

    if(return_lld) *(long long int *)target = strtoll(input, NULL, 10);
    else strcpy((char *)target, input);

    putchar('\n');
    free(input);
}

void atm_input(ATM *atm) {
    char **account_list, line[ATM_DATA_LEN + 40];
    size_t list_size = 0;

    FILE *file = fopen("account-number-old.dat", "r");

    char *ptr;
    while(fscanf(file, "%[^\n]\n", line) == 1) {
        ptr = strstr(line, "account: ");
        if(ptr != NULL) {
            if(list_size++ == 0) account_list = malloc(sizeof(char *));
            else account_list = realloc(account_list, list_size * sizeof(char *));

            account_list[list_size - 1] = malloc(ATM_ACCOUNT_LEN + 1);
            
            strncpy(account_list[list_size - 1], ptr + 9, ATM_ACCOUNT_LEN);
            account_list[list_size - 1][ATM_ACCOUNT_LEN] = '\0';
        }
    }

    fclose(file);

    atm_input_info(atm->name, ATM_NAME_LEN, NULL, 0, 0, 0, 0, 0, "Input Account Name:");
    atm_input_info(atm->account, ATM_ACCOUNT_LEN, account_list, list_size, 1, 0, 1, 0, "Input Account No:");
    atm_input_info(atm->pin, ATM_PIN_LEN, NULL, 0, 1, 0, 1, 0, "Input Pin Code:");
    atm_input_info(&(atm->balance), ATM_BALANCE_LEN, NULL, 0, 1, 1, 0, ATM_BALANCE_MIN_VALUE, "Input Balance:");
}

void fprnt_metadata(FILE *file) {
    fseek(file, 0, SEEK_SET);

    fprintf(
        file, "%d%0*d%0*d%0*d%0*d\n",
        ATM_META_LEN,
        ATM_META_LEN, ATM_NAME_LEN,
        ATM_META_LEN, ATM_ACCOUNT_LEN,
        ATM_META_LEN, ATM_PIN_LEN,
        ATM_META_LEN, ATM_BALANCE_LEN
    );
}

void atm_save(ATM *atm) {
    FILE *file = fopen("account-number-old.dat", "r+");

    // Luu do dai toi da cac thong tin cua tai khoan atm o dau file
    int ch = fgetc(file);
    if(ch == EOF) fprnt_metadata(file);
    else {
        fseek(file, 0, SEEK_SET);

        size_t cur_meta_len, cur_name_len, cur_acc_len, cur_pin_len, cur_bal_len;
        fscanf(file, "%1d", &cur_meta_len);
        fscanf(file,
            "%*d%*d%*d%*d",
            cur_meta_len, cur_name_len,
            cur_meta_len, cur_acc_len,
            cur_meta_len, cur_pin_len,
            cur_meta_len, cur_bal_len
        );

        if(!(
            cur_meta_len == ATM_META_LEN &&
            cur_name_len == ATM_NAME_LEN &&
            cur_acc_len == ATM_ACCOUNT_LEN &&
            cur_pin_len == ATM_PIN_LEN &&
            cur_bal_len == ATM_BALANCE_LEN
        )) fprnt_metadata(file);
    }

    // Kiem tra xuong dong
    fseek(file, -1, SEEK_END);
    ch = fgetc(file);
    fseek(file, 0, SEEK_END);
    if(ch != '\n') fputc('\n', file);

    // Luu thong tin tai khoan ra file
    fprintf(
        file, "NAME: %-*s ACCOUNT: %-*s PIN: %-*s BALANCE: %*lld\n",
        ATM_NAME_LEN, atm->name,
        ATM_ACCOUNT_LEN, atm->account,
        ATM_PIN_LEN, atm->pin,
        ATM_BALANCE_LEN, atm->balance
    );
    
    fclose(file);
}

void atm_randomize(ATM *atm) {
    char result[ATM_ACCOUNT_LEN + 1];
    result[0] = '\0';

    for(int i = 0; i < ATM_ACCOUNT_LEN; i+=4) {
        snprintf(result + i, ATM_ACCOUNT_LEN - i + 1, "%04d", rand() % 10000);
    }

    strcpy(atm->account, result);
}

/**
 * @brief   Take unbuffered input (instantly as user enters any key)
 * 
 * @param target        An allocated char* to store input
 * @param max_len       Maximum amount of characters to take from input
 * @param mode          1 = only 0-9, 2 = only A-z, 3 = both
 * @param is_censored   1 = show entered character as '*'
 * 
 * @return  Size of input
 */
int unbuffered_input(char *target_buffer, size_t max_buffer_size, int mode, int is_censored) {
    size_t buffer_size = 0;
    char ch, buffer[max_buffer_size + 1];

    while((ch = _getch()) != '\n' && ch != '\r') {
        switch(ch) {
            case '\b':
                if(buffer_size > 0) {
                    printf("\b \b");
                    buffer_size--;
                }
                break;

            case 27:
                while(buffer_size-- > 0) printf("\b \b");
                printf("%s", target_buffer);
                return -1;
                break;

            default:
                switch(mode) {
                    case 1: if(!isdigit(ch)) continue; break;
                    case 2: if(!isalpha(ch)) continue; break;
                    case 3: if(!isalnum(ch)) continue; break;
                }
                if(buffer_size < max_buffer_size) {
                    buffer[buffer_size++] = ch;
                    putchar(is_censored ? '*' : ch);
                }
                break;
        }
    }

    buffer[buffer_size] = '\0';
    strcpy(target_buffer, buffer);
    return buffer_size;
}

void prnt_line(size_t len, char ch) {
    while(len-- > 0) putchar(ch);
    putchar('\n');
}

void prnt_menu() {
    system("cls");
    prnt_line(UI_WIDTH, '=');
    printf("%*s\n", (UI_WIDTH + strlen(BANK_NAME)) / 2, BANK_NAME);
    prnt_line(UI_WIDTH, '=');
    printf(" Creating ATM Cards...\n");
    prnt_line(UI_WIDTH, '-');

}

void flush() {
    char ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}


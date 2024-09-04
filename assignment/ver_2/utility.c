#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#ifdef _WIN32
    #include <direct.h>  // For _mkdir on Windows
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>  // For mkdir on Unix/Linux
    #include <sys/types.h>
    #define MKDIR(dir) mkdir(dir, 0777)
#endif

// Operation states
#define OP_FINISHED      0
#define OP_LOOP        1
#define OP_CANCELLED   -1

#define UI_WIDTH 50
#define UI_PROMPT_MSG_LEN 21

#define ATM_NAME_LEN 50
#define ATM_ACCOUNT_LEN 14
#define ATM_PIN_LEN 6
#define ATM_BALANCE_LEN 18
#define ATM_BALANCE_MIN_VALUE 50000

#define ATM_NAME_TAG "NAME:"
#define ATM_ACCOUNT_TAG "ACCOUNT:"
#define ATM_PIN_TAG "PIN:"
#define ATM_BALANCE_TAG "BALANCE:"

const char *BANK_NAME = "VTC Academy Bank";
const char *FILE_NAME = "account-number.dat";

const char *DATA_TAG[4] = {ATM_NAME_TAG, ATM_ACCOUNT_TAG, ATM_PIN_TAG, ATM_BALANCE_TAG};
#define DATA_TAG_LEN strlen(DATA_TAG[0]) + strlen(DATA_TAG[1]) + strlen(DATA_TAG[2]) + strlen(DATA_TAG[3])

const int DATA_LEN[4] = {ATM_NAME_LEN, ATM_ACCOUNT_LEN, ATM_PIN_LEN, ATM_BALANCE_LEN};
#define TOTAL_DATA_LEN DATA_LEN[0] + DATA_LEN[1] + DATA_LEN[2] + DATA_LEN[3]
#define SAVE_LEN TOTAL_DATA_LEN + DATA_TAG_LEN + 7

typedef struct ATM {
    char name[ATM_NAME_LEN + 1];
    char account[ATM_ACCOUNT_LEN + 1];
    char pin[ATM_PIN_LEN + 1];
    long long int balance;
} ATM;

void init_atm_list();
void atm_append_file(ATM *atm);
void atm_append_list(ATM *atm);
void update_atm_file(int atm_index, int data_type, char *modified_data);
void random_account(char *name, char *account, char *pin, char *balance);
int validate_created_data(char *data, int data_size, int data_type);
int pin_input(char *input, int *input_size, char *ch, char *pin_to_check, int is_censored);
long long int get_money(char *input, int *input_size, char *ch);

void prnt_line(size_t len, int double_line);
void prnt_ui_line(int double_line);
void prnt_header();
void prnt_invalid(char *msg, int input_size, char *ch);

void standardize_str(char *str);
void str_to_money(char *target_money_str, long long int money);
void receipt(long long int withdraw_amount);
int unbuffered_input(char *target_buffer, int max_size, int input_mode, int is_censored, char first_ch);
int choice_input(int min, int max);
int yes_no_input();

ATM *cur_atm_ptr, *atm_list = NULL;
size_t atm_list_size = 0, atm_list_buffer_size = 10;
int cur_atm_index = -1;

/**
 * @brief    Initialize atm_list, should be called once at the start
 */
void init_atm_list() {
    FILE *file = fopen(FILE_NAME, "r");

    if(file == NULL || feof(file)) return;

    if(atm_list == NULL) atm_list = malloc(atm_list_buffer_size * sizeof(ATM));
    ATM temp_atm;

    char format[60], line[150], buffer[100];
    sprintf(format, "%s %%%d[^\n] %s %%%ds %s %%%ds %s %%%d[^\n]\n",
        DATA_TAG[0], DATA_LEN[0],
        DATA_TAG[1], DATA_LEN[1],
        DATA_TAG[2], DATA_LEN[2],
        DATA_TAG[3], DATA_LEN[3]
    );
    while(fscanf(file, format, temp_atm.name, temp_atm.account, temp_atm.pin, buffer) == 4) {
        char *trim_ptr = temp_atm.name + ATM_NAME_LEN;
        while(isspace(*--trim_ptr));
        *(trim_ptr + 1) = '\0';
        temp_atm.balance = strtoll(buffer, NULL, 10);
        atm_append_list(&temp_atm);
    }

    fclose(file);
}

void atm_append_file(ATM *atm) {
    FILE *file = fopen(FILE_NAME, "a");

    fprintf(file,
        "%s %-*s %s %*s %s %*s %s %-*lld\n",
        DATA_TAG[0], DATA_LEN[0], atm->name,
        DATA_TAG[1], DATA_LEN[1], atm->account,
        DATA_TAG[2], DATA_LEN[2], atm->pin,
        DATA_TAG[3], DATA_LEN[3], atm->balance
    );

    fclose(file);
}

void atm_append_list(ATM *atm) {
    strcpy(atm_list[atm_list_size].name, atm->name);
    strcpy(atm_list[atm_list_size].account, atm->account);
    strcpy(atm_list[atm_list_size].pin, atm->pin);
    atm_list[atm_list_size].balance = atm->balance;

    if(++atm_list_size >= atm_list_buffer_size) {
        atm_list_buffer_size *= 2;
        atm_list = realloc(atm_list, atm_list_buffer_size * sizeof(ATM));
    }
}

/**
 * @brief   Change indexed account's one specific data in save file
 * 
 * @param atm_index     Account index
 * @param data_type     0: name, 1: account, 2: pin, 3: balance
 * @param modified_data String of value to change
 */
void update_atm_file(int atm_index, int data_type, char *modified_data) {
    FILE *file = fopen("account-number.dat", "r+");

    for(int i = 0; i < atm_index; i++) fscanf(file, "%*[^\n]\n");

    char line[SAVE_LEN + 1], *tag_ptr;
    fscanf(file, "%[^\n]", line);

    if((tag_ptr = strstr(line, DATA_TAG[data_type])) != NULL) {
        fseek(file, - strlen(line) + (tag_ptr - line) + strlen(DATA_TAG[data_type]) + 1, SEEK_CUR);
        fprintf(file, "%-*s", DATA_LEN[data_type], modified_data);
    }

    fclose(file);
}

void random_account(char *name, char *account, char *pin, char *balance) {
    static const char *last_names[] = {"Nguyen", "Tran", "Le", "Hoang", "Pham", "Dang", "Ngo"};
    static const char *middle_names[] = {"Gia", "Tra", "Thanh", "Van", "Thi", "Duong", "Linh"};
    static const char *first_names[] = {"Anh", "Ha", "Hoa", "Ly", "Giang", "Kim", "Tu", "Linh"};

    int lname_rand = rand() % (sizeof(last_names) / sizeof(last_names[0]));
    int mname_rand = rand() % (sizeof(middle_names) / sizeof(middle_names[0]));
    int fname_rand = rand() % (sizeof(first_names) / sizeof(first_names[0]));

    sprintf(name, "%s %s %s", last_names[lname_rand], middle_names[mname_rand], first_names[fname_rand]);
    sprintf(account, "%04d%04d%04d%02d", rand() % 10000, rand() % 10000, rand() % 10000, rand() % 100);
    sprintf(pin, "%03d%03d", rand() % 1000, rand() % 1000);
    sprintf(balance, "%d00000", rand() % 10000);
}

/**
 * @param data_type    0: name, 1: account, 2: pin, 3: balance
 * 
 * @return  0: invalid, 1: valid (balance > min), -1: duped acc
 */
int validate_created_data(char *data, int data_size, int data_type) {
    if(data_type == 1) for(int i = 0; i < atm_list_size; i++) if(strcmp(data, atm_list[i].account) == 0) return -1;
    if(data_type == 3 && strtoll(data, NULL, 10) < ATM_BALANCE_MIN_VALUE) return 0;
    return (data_type == 1 || data_type == 2) ? data_size == DATA_LEN[data_type] : (data_size > 0 && data_size <= DATA_LEN[data_type]);
}

/**
 * @brief   Keep taking input until valid PIN or ESC entered
 * 
 * @param input             Allocated char* to store input
 * @param input_size        int* to store input size
 * @param ch                ch* to store first character entered
 * @param pin_to_check      If !NULL: used to compare entered PIN
 * @param is_censored       If 1: show entered PIN as '*'
 * 
 * @return  OP states: 0, 1
 */
int pin_input(char *input, int *input_size, char *ch, char *pin_to_check, int is_censored) {
    *ch = 0;
    while(1) {
        if((*input_size = unbuffered_input(input, DATA_LEN[2], 1, is_censored, *ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(*input_size == DATA_LEN[2]) {
            if(pin_to_check != NULL && strcmp(input, pin_to_check) != 0) {
                prnt_invalid("Incorrect PIN", *input_size, ch);
                continue;
            }
            break;
        }

        prnt_invalid("Invalid PIN", *input_size, ch);
    }
    putchar('\n');
    return OP_FINISHED;
}

/**
 * @brief   Withdraw money from cur_atm->balance, keep taking input until valid amount or ESC entered
 * 
 * @return  Withdraw amount, OP_CANCELLED if ESC
 */
long long int get_money(char *input, int *input_size, char *ch) {
    long long int withdraw_amount;
    *ch = 0;

    printf(" Enter Money Ammount: ");
    while(1) {
        if((*input_size = unbuffered_input(input, DATA_LEN[3], 1, 0, *ch)) == -1) return OP_CANCELLED;
        if(*input_size > 0 && *input_size <= DATA_LEN[3]) {
            if(strtoll(input, NULL, 10) > 0 && strtoll(input, NULL, 10) <= cur_atm_ptr->balance) {
                withdraw_amount = strtoll(input, NULL, 10);
                break;
            }
            else {
                prnt_invalid("Not enough money in account", *input_size, ch);
                continue;
            }
        }

        prnt_invalid("Invalid ammount", *input_size, ch);
    }
    putchar('\n');

    return withdraw_amount;
}

void prnt_line(size_t len, int double_line) {
    static char ch;
    ch = double_line ? '=' : '-';
    while(len-- > 0) putchar(ch);
    putchar('\n');
}

void prnt_ui_line(int double_line) {
    prnt_line(UI_WIDTH, double_line);
}

void prnt_invalid(char *msg, int input_size, char *ch) {
    while(input_size-- > 0) printf("\b \b");
    printf("%s", msg);
    *ch = _getch();
    for(int i = 0; i < strlen(msg); i++) printf("\b \b");
}

void prnt_header() {
    prnt_ui_line(1);
    printf("%*s\n", (UI_WIDTH + strlen(BANK_NAME)) / 2, BANK_NAME);
    prnt_ui_line(1);
}

/**
 * @brief   Remove excess spaces, first character of words to upper, otherwise to lower
 */
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

    // *str = toupper(*str);
    ptr -= !is_word;
    *ptr = '\0';
}

/**
 * @brief   Add dot every 3 digits and "VND" at the end
 */
void str_to_money(char *target_money_str, long long int money) {
    char money_str[ATM_BALANCE_LEN + 10];
    sprintf(money_str, "%lld", money);
    size_t len = strlen(money_str);

    int dot_count = 0;
    for(int i = 0; i < len; i++) {
        if((len - i) % 3 == 0 && i) {
            memmove(money_str + i + dot_count + 1, money_str + i + dot_count, len - i);
            money_str[i + dot_count++] = '.';
        }
    }
    money_str[len + dot_count] = '\0';
    strcat(money_str, " VND");
    strcpy(target_money_str, money_str);
}

void str_to_line(char *target, int double_line) {
    int ch = double_line ? '=' : '-';
    for(int i = 0; i < UI_WIDTH; i++) target[i] = ch;
    target[UI_WIDTH] = '\n';
    target[UI_WIDTH + 1] = '\0';
}

void receipt(long long int withdraw_amount) {
    system("cls");

    char result_str[(UI_WIDTH + 1) * 18 + 1], line_write[UI_WIDTH + 2], buffer[101], buffer2[101];

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(BANK_NAME)) / 2, BANK_NAME);
    strcat(result_str, line_write);

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    sprintf(buffer, "BIEN LAI RUT TIEN TAI ATM");
    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(buffer)) / 2, buffer);
    strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    
    strftime(buffer, 100, " Ngay: %x", tm_info);
    strftime(buffer2, 100, "Gio: %X", tm_info);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " So the:");
    strcpy(buffer2, cur_atm_ptr->account);
    memset(buffer2 + 4, '*', DATA_LEN[1] - 8);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " So giao dich:");
    strftime(buffer2, 100, "%y%m%d%H%M%S", tm_info);
    sprintf(buffer2 + strlen(buffer2), "%d", rand() % 10000);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    char receipt_name[100];
    sprintf(receipt_name, "receipts/receipt-%s.txt", buffer2);
    MKDIR("receipts");

    sprintf(buffer, " Noi dung:");
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), "Rut tien mat tai AMT");
    strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    sprintf(buffer, " So tien:");
    str_to_money(buffer2, withdraw_amount);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " Le phi:");
    str_to_money(buffer2, 1000);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " VAT:");
    str_to_money(buffer2, 100);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " So du:");
    str_to_money(buffer2, cur_atm_ptr->balance);
    sprintf(line_write, "%s%*s\n", buffer, UI_WIDTH - strlen(buffer), buffer2);
    strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    char end_msg_1[] = "Cam on quy khach da su dung",
        end_msg_2[] = "dich vu cua chung toi!";
    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(end_msg_1)) / 2, end_msg_1);
    strcat(result_str, line_write);

    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(end_msg_2)) / 2, end_msg_2);
    strcat(result_str, line_write);

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    printf("%s", result_str);

    FILE *file = fopen(receipt_name, "w");
    fprintf(file, "%s", result_str);
    fclose(file);
}

void free_everything() {
    free(atm_list);
}

/**
 * @brief   Process unbuffered input (instantly as user enters any key)
 * 
 * @param target_buffer     An allocated char* to store input
 * @param max_size          Maximum amount of characters to take from input
 * @param input_mode        Type of input, 1: only 0-9, 2: only A-z, 3: both
 * @param is_censored       If 1: show entered character as '*'
 * @param first_ch          If !0: take this as first char
 * 
 * @return  Size of input, OP_CANCELLED if ESC
 */
int unbuffered_input(char *target_buffer, int max_size, int input_mode, int is_censored, char first_ch) {
    int input_size = 0;
    char ch, input[max_size + 1];

    while(1) {
        if(first_ch != 0) {
            ch = first_ch;
            first_ch = 0;
        } else ch = _getch();

        if(ch == '\n' || ch == '\r') break;
        switch(ch) {
            case 23:
                while(input_size > 0) {
                    printf("\b \b");
                    input_size--;
                }
                break;

            case '\b':
                if(input_size > 0) {
                    printf("\b \b");
                    input_size--;
                }
                break;

            case 27:
                while(input_size-- > 0) printf("\b \b");
                return OP_CANCELLED;
                break;

            default:
                switch(input_mode) {
                    case 1: if(!isdigit(ch)) continue; break;
                    case 2: if(!isalpha(ch) && !(isspace(ch) && input_size > 0)) continue; break;
                    case 3: if(!isalnum(ch) && !(isspace(ch) && input_size > 0)) continue; break;
                    default: continue; break;
                }
                if(input_size < max_size) {
                    input[input_size++] = ch;
                    putchar(is_censored ? '*' : ch);
                }
                break;
        }
    }
    
    input[input_size] = '\0';
    strcpy(target_buffer, input);
    return input_size;
}

/**
 * @brief   Keep taking input until range meet or ESC entered. Print "Your choice: " automatically
 * 
 * @return  Min to max, OP_CANCELLED if ESC
 */
int choice_input(int min, int max) {
    char input[2] = "\0", ch = 0;
    int choice, input_len;

    printf(" Your choice: ");
    while (1) {
        input_len = unbuffered_input(input, 1, 1, 0, ch);
        choice = input_len > 0 ? strtol(input, NULL, 10) : -1;

        if(input_len == -1) return OP_CANCELLED;
        if(choice >= min && choice <= max) break;

        prnt_invalid("Invalid choice", input_len, &ch);
    }
    putchar('\n');

    return choice;
}

/**
 * @brief   Keep taking input until 'Y', 'y', 'N', 'n', ESC entered
 * 
 * @return  1: yes, 0: no & ESC
 */
int yes_no_input() {
    int input_size;
    char input[2];
    while(1) {
        input[0] = '\0';
        input_size = unbuffered_input(input, 1, 2, 0, 0);
        if(input[0] == 'Y' || input[0] == 'y') {
            putchar('\n');
            return 1;
        }
        if(input[0] == 'N' || input[0] == 'n' || input_size == -1) {
            putchar('\n');
            return 0;
        }
        printf("\b %s", input_size > 0 ? "\b" : "");
    }
}

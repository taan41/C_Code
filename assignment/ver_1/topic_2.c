#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#include <time.h>

typedef struct ATM
{
    char *name;
    char *account;
    char *pin;
    long long int balance;
} ATM;

const size_t UI_WIDTH = 50;
const size_t UI_NOTI_LEN = 50;
const size_t UI_LOGIN_PROMPT_LEN = 20;
const size_t UI_CHECK_ACC_PROMPT_LEN = 20;
const size_t UI_CHANGE_PIN_PROMPT_LEN = 20;

const char *BANK_NAME = "VTC Academy Bank";
const char *MENU[] = {
    "End transaction",
    "Check account's balance",
    "Withdraw money",
    "Transfer money to another account",
    "Change PIN"
};

size_t ATM_META_LEN;
size_t ATM_NAME_LEN;
size_t ATM_ACCOUNT_LEN;
size_t ATM_PIN_LEN;
size_t ATM_BALANCE_LEN;
#define ATM_DATA_LEN ATM_NAME_LEN + ATM_ACCOUNT_LEN + ATM_PIN_LEN + ATM_BALANCE_LEN

ATM *ATM_LIST, *CUR_ATM;
size_t ATM_LIST_SIZE = 0;
int CUR_ATM_INDEX;

void check_acc();
void withdraw();
void receipt(long long int withdraw_amount);
void transfer();
void change_pin();

int log_in();
void load_atm_list();
void modify_file(int atm_index, char *data_name, size_t data_len, char *modded_data);

void str_to_money(char *target_money_str, long long int money);
size_t input_unbuffered(char *target, size_t len, int digit_only, int hiding, char *prompt_msg, size_t prompt_len);

void prnt_invalid_prompt(char *prompt_msg, char *invalid_msg, size_t prompt_len, size_t input_len);
void prnt_line(size_t len, char ch);
void prnt_header();
void prnt_menu();
void prnt_main_screen();
void flush();

int main() {
    srand(time(NULL));

    load_atm_list();

    if(!log_in()) {
        printf("\nLog in error\n");
        return 1;
    }

    int choice, choice_len;
    char choice_buffer[2];
    prnt_main_screen();
    printf(" Your choice: ");
    while(1) {
        
        choice_len = input_unbuffered(choice_buffer, 2, 1, 0, "Your choice: ", 0);
        choice = choice_len == 1 ? atoi(choice_buffer) : -1;

        switch(choice) {
            case 1:
                check_acc();
                goto Main_screen_label;

            case 2:
                withdraw();
                goto Main_screen_label;

            case 3:
                transfer();
                goto Main_screen_label;

            case 4:
                change_pin();

            Main_screen_label:
                prnt_main_screen();
                printf(" Your choice: ");
                break;

            case 0:
                printf("\n Ending transaction...");
                exit(0);
                break;

            default:
                printf("\r%*s\r Your choice: Invalid choice", 17, "");
                break;
        }
    }

    return 0;
}

void check_acc() {
    system("cls");
    prnt_header();
    printf(" Checking account's balance...\n");
    prnt_line(UI_WIDTH, '-');

    char balance[ATM_BALANCE_LEN + 5];
    str_to_money(balance, CUR_ATM->balance);
    printf(
        " %-*s%s\n %-*s%s\n %-*s%s\n",
        UI_CHECK_ACC_PROMPT_LEN, "Account name:", CUR_ATM->name,
        UI_CHECK_ACC_PROMPT_LEN, "Account No:", CUR_ATM->account,
        UI_CHECK_ACC_PROMPT_LEN, "Current balance:" , balance
    );

    prnt_line(UI_WIDTH, '-');
    printf(" Press any key to continue...");
    _getch();
}

void withdraw() {
    long long int withdraw_amount;
    int choice, choice_len = 0, end = 0, done_withdraw = 0;

    char balance[ATM_BALANCE_LEN + 5], choice_buffer[2];

    size_t input_len;
    char *input = malloc(ATM_BALANCE_LEN + 1);
    strcpy(input, "0");

    while(!end) {
        system("cls");
        prnt_header();

        str_to_money(balance, CUR_ATM->balance);
        printf(
            " Withdrawing money...\n"
            " Current balance: %s\n",
            balance
        );

        prnt_line(UI_WIDTH, '-');
        printf(
            " 1.   100.000 VND\n"
            " 2.   200.000 VND\n"
            " 3.   500.000 VND\n"
            " 4. 1.000.000 VND\n"
            " 5. 2.000.000 VND\n"
            " 6. Other amount\n"
            " 0. Return\n"
        );
        prnt_line(UI_WIDTH, '-');
        printf(" Your choice: ");

        done_withdraw = 0;
        while(!done_withdraw) {
            choice_len = input_unbuffered(choice_buffer, 2, 1, 0, "Your choice: ", 0);
            choice = choice_len == 1 ? atoi(choice_buffer) : -1;

            switch(choice) {
                case 1:
                    withdraw_amount = 100000;
                    goto withdraw_label;

                case 2:
                    withdraw_amount = 200000;
                    goto withdraw_label;

                case 3:
                    withdraw_amount = 500000;
                    goto withdraw_label;

                case 4:
                    withdraw_amount = 1000000;
                    goto withdraw_label;

                case 5:
                    withdraw_amount = 2000000;
                    goto withdraw_label;

                case 6:
                    printf("\r%*s\r Enter amount: ", 17 + ATM_BALANCE_LEN, "");
                    while(1) {
                        input_len = input_unbuffered(input, ATM_BALANCE_LEN, 1, 0, "Enter amount: ", 0);

                        if(!input_len) prnt_invalid_prompt("Enter amount: ", "Invalid amount", 16, input_len);
                        else {
                            withdraw_amount = strtoll(input, NULL, 10);
                            break;
                        }
                    }

                withdraw_label:
                    if (withdraw_amount > CUR_ATM->balance) {
                        printf(
                            "\r%*s\r Not enough money in account!\n"
                            " Press any key to continue...",
                            ATM_BALANCE_LEN + 15, ""
                        );
                        done_withdraw = 1;
                    }
                    else {
                        char confirm, withdraw_amount_str[ATM_BALANCE_LEN + 5];
                        str_to_money(withdraw_amount_str, withdraw_amount);
                        printf(
                            "\r%*s\r Withdrawing %s...\n"
                            " Please confirm (Y/N): ",
                            17, "", withdraw_amount_str
                        );
                        confirm = getchar();
                        flush();

                        prnt_line(UI_WIDTH, '-');
                        if(confirm == 'Y' || confirm == 'y') {
                            printf(
                                " Successfully withdrawed %s!\n"
                                " Do you want to print receipt? (Y/N): ",
                                withdraw_amount_str
                            );
                            confirm = getchar();
                            flush();
                            if(confirm == 'Y' || confirm == 'y') {
                                CUR_ATM->balance -= 1100;
                                receipt(withdraw_amount);
                            }

                            CUR_ATM->balance -= withdraw_amount;
                            char balance_str[ATM_BALANCE_LEN + 1];
                            sprintf(balance_str, "%*lld", ATM_BALANCE_LEN, CUR_ATM->balance);
                            modify_file(CUR_ATM_INDEX, "BALANCE: ", ATM_BALANCE_LEN, balance_str);
                        }
                        else {
                            printf(" Withdrawal cancelled.\n");
                        }

                        printf(" Press any key to continue...");
                        done_withdraw = 1;
                    }

                    _getch();
                    break;

                case 0:
                    printf(
                        "\r%*s\r Return to main menu.\n"
                        " Press any key to continue...",
                        17, ""
                    );
                    _getch();
                    done_withdraw = 1;
                    end = 1;
                    break;

                default:
                    printf("\r%*s\r Your choice: Invalid choice", 17, "");
                    break;
            }
        }
    }
    free(input);
}

void receipt(long long int withdraw_amount) {
    system("cls");
    prnt_header();

    char header[] = "BIEN LAI RUT TIEN TAI ATM";
    printf("%*s%s\n", (UI_WIDTH - strlen(header)) / 2, "", header);
    prnt_line(UI_WIDTH, '-');

    char buffer[100];

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    
    strftime(buffer, 100, "Gio: %X", tm_info);
    printf("%*s", UI_WIDTH, buffer);
    strftime(buffer, 100, "%x", tm_info);
    printf("\r Ngay: %s\n", buffer);

    strcpy(buffer, CUR_ATM->account);
    memset(buffer + 4, '*', ATM_ACCOUNT_LEN - 8);
    printf("%*s\r So the:\n", UI_WIDTH, buffer);

    strftime(buffer, 100, "%y%m%d%H%M%S", tm_info);
    sprintf(buffer + strlen(buffer), "%d", rand() % 10000);
    printf("%*s\r So giao dich:\n", UI_WIDTH, buffer);

    printf("%*s\r Noi dung:\n", UI_WIDTH, "Rut tien mat tai ATM");
    prnt_line(UI_WIDTH, '-');

    str_to_money(buffer, withdraw_amount);
    printf("%*s\r So tien:\n", UI_WIDTH, buffer);
    str_to_money(buffer, 1000);
    printf("%*s\r Le phi:\n", UI_WIDTH, buffer);
    str_to_money(buffer, 100);
    printf("%*s\r VAT:\n", UI_WIDTH, buffer);
    str_to_money(buffer, CUR_ATM->balance);
    printf("%*s\r So du:\n", UI_WIDTH, buffer);

    prnt_line(UI_WIDTH, '-');
    char end_msg_1[] = "Cam on quy khach da su dung",
        end_msg_2[] = "dich vu cua chung toi!";
    printf("%*s%s\n", (UI_WIDTH - strlen(end_msg_1)) / 2, "", end_msg_1);
    printf("%*s%s\n", (UI_WIDTH - strlen(end_msg_2)) / 2, "", end_msg_2);
    prnt_line(UI_WIDTH, '=');
}

void transfer() {
    long long int transfer_amount;
    int done_transfer = 0, found_target = 0, target_index;

    size_t buffer_size;
    char buffer[100], target_acc[ATM_ACCOUNT_LEN + 1];
    strcpy(buffer, "0");

    system("cls");
    prnt_header();

    str_to_money(buffer, CUR_ATM->balance);
    printf(
        " Transfering money...\n"
        " Current balance: %s\n",
        buffer
    );
    prnt_line(UI_WIDTH, '-');

    printf(" Transfer to account: ");
    while(!found_target) {
        buffer_size = input_unbuffered(buffer, ATM_ACCOUNT_LEN, 1, 0, "Transfer to account: ", 0);

        if(buffer_size == ATM_ACCOUNT_LEN || buffer_size == -1) {
            for(int i = 0; i < ATM_LIST_SIZE; i++) {
                if(!strcmp(buffer, ATM_LIST[i].account)) {
                    strcpy(target_acc, ATM_LIST[i].account);
                    target_index = i;
                    found_target = 1;
                    break;
                }
            }

            if(found_target) putchar('\n');
            else prnt_invalid_prompt("Transfer to account: ", "Account not found", 0, buffer_size);
        }
        else prnt_invalid_prompt("Transfer to account: ", "Invalid account", 0, buffer_size);
    }

    strcpy(buffer, "1000");
    printf(" Transfer amount: ");
    while(!done_transfer) {
        buffer_size = input_unbuffered(buffer, ATM_BALANCE_LEN, 1, 0, "Transfer amount: ", 0);

        if(buffer_size > 0 || buffer_size == -1) {
            transfer_amount = strtoll(buffer, NULL, 10);

            if(transfer_amount > CUR_ATM->balance) {
                prnt_invalid_prompt("Transfer amount: ", "Not enough money", 0, buffer_size);
            }
            else {
                char balance_str[ATM_BALANCE_LEN + 1];

                CUR_ATM->balance -= transfer_amount;
                sprintf(balance_str, "%*lld", ATM_BALANCE_LEN, CUR_ATM->balance);
                modify_file(CUR_ATM_INDEX, "BALANCE: ", ATM_BALANCE_LEN, balance_str);

                ATM_LIST[target_index].balance += transfer_amount;
                sprintf(balance_str, "%*lld", ATM_BALANCE_LEN, ATM_LIST[target_index].balance);
                modify_file(target_index, "BALANCE: ", ATM_BALANCE_LEN, balance_str);

                done_transfer = 1;
                putch('\n');
            }
        }
        else prnt_invalid_prompt("Transfer amount: ", "Invalid amount", 0, buffer_size);
    }

    prnt_line(UI_WIDTH, '-');
    str_to_money(buffer, transfer_amount);
    printf(
        " Successfully transfered %s!\n"
        " Press any key to continue...",
        buffer
    );
    _getch();
}

void change_pin() {
    system("cls");
    prnt_header();
    printf(" Changing PIN...\n");
    prnt_line(UI_WIDTH, '-');

    size_t input_len;
    char *input = malloc(ATM_PIN_LEN + 1);
    strcpy(input, "000000");
    printf(" %-*s", UI_CHANGE_PIN_PROMPT_LEN, "Input current PIN:");
    while(1) {
        input_len = input_unbuffered(input, ATM_PIN_LEN, 1, 1, "Input current PIN:", UI_CHANGE_PIN_PROMPT_LEN);

        if(input_len == ATM_PIN_LEN || input_len == -1) {
            if(!strcmp(input, CUR_ATM->pin)) {
                putchar('\n');
                break;
            }
            else prnt_invalid_prompt("Input current PIN:", "Incorrect PIN", UI_CHANGE_PIN_PROMPT_LEN, input_len);
        }
        else prnt_invalid_prompt("Input current PIN:", "Invalid PIN", UI_CHANGE_PIN_PROMPT_LEN, input_len);
    }

    char *new_pin = malloc(ATM_PIN_LEN + 1);
    strcpy(input, "000000");
    printf(" %-*s", UI_CHANGE_PIN_PROMPT_LEN, "Input new PIN:");
    while(1) {
        input_len = input_unbuffered(input, ATM_PIN_LEN, 1, 0, "Input new PIN:", UI_CHANGE_PIN_PROMPT_LEN);

        if(input_len == ATM_PIN_LEN || input_len == -1) {
            putchar('\n');
            break;
        }
        else prnt_invalid_prompt("Input new PIN:", "Invalid PIN", UI_CHANGE_PIN_PROMPT_LEN, input_len);
    }
    strcpy(new_pin, input);
    
    strcpy(input, "000000");
    printf(" %-*s", UI_CHANGE_PIN_PROMPT_LEN, "Confirm new PIN:");
    while(1) {
        input_len = input_unbuffered(input, ATM_PIN_LEN, 1, 0, "Confirm new PIN:", UI_CHANGE_PIN_PROMPT_LEN);

        if(input_len == ATM_PIN_LEN || input_len == -1) {
            if(!strcmp(input, new_pin)) {
                putchar('\n');
                break;
            }
            else prnt_invalid_prompt("Confirm new PIN:", "Incorrect PIN", UI_CHANGE_PIN_PROMPT_LEN, input_len);
        }
        else prnt_invalid_prompt("Confirm new PIN:", "Invalid PIN", UI_CHANGE_PIN_PROMPT_LEN, input_len);
    }
    strcpy(CUR_ATM->pin, new_pin);
    modify_file(CUR_ATM_INDEX, "PIN: ", ATM_PIN_LEN, new_pin);

    prnt_line(UI_WIDTH, '-');
    printf(
        " Changed PIN successfully!\n"
        " Press any key to continue..."
    );
    _getch();
    free(input);
}

int log_in() {
    prnt_header();
    printf(" Logging in an account\n");
    prnt_line(UI_WIDTH, '-');

    int valid_acc = 0;
    size_t input_len = 0;
    char *input = malloc(ATM_ACCOUNT_LEN + 1);
    strcpy(input, "00000000000000");

    printf(" %-*s", UI_LOGIN_PROMPT_LEN, "Input Account No:");

    while(!valid_acc) {
        while(1) {
            input_len = input_unbuffered(input, ATM_ACCOUNT_LEN, 1, 0, "Input Account No:", UI_LOGIN_PROMPT_LEN);
            
            if(input_len == ATM_ACCOUNT_LEN || input_len == -1) {
                for(int i = 0; i < ATM_LIST_SIZE; i++) {
                    if(strcmp(input, ATM_LIST[i].account) == 0) {
                        valid_acc = 1;
                        CUR_ATM = &ATM_LIST[i];
                        CUR_ATM_INDEX = i;
                        break;
                    }
                }

                if(valid_acc) {
                    putchar('\n');
                    break;
                }
                else prnt_invalid_prompt("Input Account No:", "Account not found", UI_LOGIN_PROMPT_LEN, input_len);
            }
            else prnt_invalid_prompt("Input Account No:", "Invalid account", UI_LOGIN_PROMPT_LEN, input_len);

        }
    }

    input = realloc(input, ATM_PIN_LEN + 1);
    strcpy(input, "000000");

    printf(" %-*s", UI_LOGIN_PROMPT_LEN, "Input PIN:");

    while(1) {
        input_len = input_unbuffered(input, ATM_PIN_LEN, 1, 1, "Input PIN:", UI_LOGIN_PROMPT_LEN);

        if(input_len == ATM_PIN_LEN|| input_len == -1) {
            if(!strcmp(input, CUR_ATM->pin)) {
                putchar('\n');
                break;
            }
            else prnt_invalid_prompt("Input PIN:", "Incorrect PIN", UI_LOGIN_PROMPT_LEN, input_len);
        }
        else prnt_invalid_prompt("Input PIN:", "Invalid PIN", UI_LOGIN_PROMPT_LEN, input_len);
    }

    free(input);
    return 1;
}

void load_atm_list() {
    FILE *file = fopen("account-number-old.dat", "r");

    if(file == NULL) {
        printf("No save file found.\n");
        return;
    }
    char ch = fgetc(file);
    if(ch == EOF) {
        printf("Save file is emtpy.\n");
        return;
    }

    // Lay thong tin metadata
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%1d", &ATM_META_LEN);

    char meta_fscanf_format[14];
    sprintf(meta_fscanf_format, "%%%dd%%%dd%%%dd%%%dd\n\0", ATM_META_LEN, ATM_META_LEN, ATM_META_LEN, ATM_META_LEN);
    fscanf(
        file, meta_fscanf_format,
        &ATM_NAME_LEN,
        &ATM_ACCOUNT_LEN,
        &ATM_PIN_LEN,
        &ATM_BALANCE_LEN
    );

    char line[ATM_DATA_LEN + 40], *data_ptr;
    while(fscanf(file, "%[^\n]\n", line) == 1) {
        data_ptr = strstr(line, "NAME: ");
        if(data_ptr != NULL) {
            if(ATM_LIST_SIZE++ == 0) ATM_LIST = malloc(sizeof(ATM));
            else ATM_LIST = realloc(ATM_LIST, ATM_LIST_SIZE * sizeof(ATM));

            ATM_LIST[ATM_LIST_SIZE - 1].name = malloc(ATM_NAME_LEN + 1);

            strncpy(ATM_LIST[ATM_LIST_SIZE - 1].name, data_ptr + 6, ATM_NAME_LEN);

            char *back_ptr = &ATM_LIST[ATM_LIST_SIZE - 1].name[ATM_NAME_LEN];
            while(isspace(*--back_ptr));
            *(back_ptr + 1) = '\0';

        } else continue;

        data_ptr = strstr(line, "ACCOUNT: ");
        if(data_ptr != NULL) {
            ATM_LIST[ATM_LIST_SIZE - 1].account = malloc(ATM_ACCOUNT_LEN + 1);

            strncpy(ATM_LIST[ATM_LIST_SIZE - 1].account, data_ptr + 9, ATM_ACCOUNT_LEN);
            ATM_LIST[ATM_LIST_SIZE - 1].account[ATM_ACCOUNT_LEN] = '\0';
        }

        data_ptr = strstr(line, "PIN: ");
        if(data_ptr != NULL) {
            ATM_LIST[ATM_LIST_SIZE - 1].pin = malloc(ATM_PIN_LEN + 1);

            strncpy(ATM_LIST[ATM_LIST_SIZE - 1].pin, data_ptr + 5, ATM_PIN_LEN);
            ATM_LIST[ATM_LIST_SIZE - 1].pin[ATM_PIN_LEN] = '\0';
        }
        
        data_ptr = strstr(line, "BALANCE: ");
        if(data_ptr != NULL) {
            char balance_str[ATM_BALANCE_LEN + 1];
            strncpy(balance_str, data_ptr + 9, ATM_BALANCE_LEN);
            balance_str[ATM_BALANCE_LEN] = '\0';

            ATM_LIST[ATM_LIST_SIZE - 1].balance = strtoll(balance_str, NULL, 10);
        }
    }

    fclose(file);
}

void modify_file(int atm_index, char *data_name, size_t data_len, char *modded_data) {
    FILE *file = fopen("account-number-old.dat", "r+");

    for(int i = 0; i < atm_index + 1; i++) fscanf(file, "%*[^\n]\n");

    char line_buffer[ATM_DATA_LEN + 39], *data_ptr;
    fscanf(file, "%[^\n]", line_buffer);

    if((data_ptr = strstr(line_buffer, data_name)) != NULL) {
        fseek(file, -strlen(line_buffer) + (data_ptr - line_buffer) + strlen(data_name), SEEK_CUR);
        fprintf(file, "%-*s", data_len, modded_data);
    }

    fclose(file);
}

/**
 * @brief   Take unbuffered input (instantly as user enters)
 * 
 * @param target        String to save the input
 * @param len           Max length of target string ('\0' at the end)
 * @param digit_only    TRUE: only accept 0-9, FALSE: only accept A-z
 * @param hiding        TRUE: show entered char as '*', FALSE: show entered char as is
 * @param prompt_msg    Prompt msg to show before taking input
 * @param prompt_len    prompt msg length
 */
size_t input_unbuffered(char *target, size_t len, int digit_only, int hiding, char *prompt_msg, size_t prompt_len) {
    size_t input_len = 0;
    char ch, input[len + 1];
    input[0] = '\0';

    ch = _getch();
    printf("\r %*s\r %-*s", prompt_len + 50, "", prompt_len, prompt_msg);

    while(1) {
        if((digit_only ? isdigit(ch) : (isalpha(ch) || ch == ' ')) && input_len < len) {
            input[input_len++] = ch;

            if(hiding) putchar('*');
            else putchar(ch);
        }
        else if(ch == '\n' || ch == '\r') {
            input[input_len] = '\0';
            break;
        }
        else if(ch == '\b' && input_len > 1) {
            printf("\b \b");
            input_len--;
        }
        else if(ch == 27) {
            printf(
                "\r %*s\r %-*s%s",
                prompt_len + input_len, "",
                prompt_len, prompt_msg,
                target
            );

            return -1;
        }

        ch = _getch();
    }

    strcpy(target, input);
    return input_len;
}

void prnt_invalid_prompt(char *prompt_msg, char *invalid_msg, size_t prompt_len, size_t input_len) {
    printf(
        "\r %*s\r %-*s%s",
        prompt_len + input_len, "",
        prompt_len, prompt_msg,
        invalid_msg
    );
}

void prnt_line(size_t len, char ch) {
    while(len-- > 0) putchar(ch);
    putchar('\n');
}

void prnt_header() {
    prnt_line(UI_WIDTH, '=');
    printf("%*s%s\n", (UI_WIDTH - strlen(BANK_NAME)) / 2, "", BANK_NAME);
    prnt_line(UI_WIDTH, '=');
}

void prnt_menu() {
    prnt_line(UI_WIDTH, '-');
    for(int i = 1; i < sizeof(MENU) / sizeof(MENU[0]); i++) printf(" %d. %s\n", i, MENU[i]);
    printf(" 0. %s\n", MENU[0]);
    prnt_line(UI_WIDTH, '-');
}

void prnt_main_screen() {
    system("cls");
    prnt_header();
    printf(" Welcome %s!\n", CUR_ATM->name);
    printf(" Account No: %s\n", CUR_ATM->account);

    prnt_menu();
}

void str_to_money(char *target_money_str, long long int money) {
    char money_str[ATM_BALANCE_LEN + 5];
    sprintf(money_str, "%lld", money);
    size_t len = strlen(money_str);

    int dot_count = 0;
    for(int i = 0; i < len; i++) {
        if(!((len - i) % 3) && i) {
            memmove(money_str + i + dot_count + 1, money_str + i + dot_count, len - i);
            money_str[i + dot_count++] = '.';
        }
    }
    money_str[len + dot_count] = '\0';
    strcat(money_str, " VND");
    strcpy(target_money_str, money_str);
}

void flush() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

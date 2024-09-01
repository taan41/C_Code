#include "manager_ui_utility.c"

// Defines

#define SAVE_FILE "data\\account-number.dat"

#define DEFAULT_ATTEMPTS_AMOUNT 7

typedef struct ATM {
    char *name;
    char *account;
    char *pin;
    long long int balance;
    int PIN_attempt;
} ATM;

// Function prototypes

void init_data_manager();
void init_atm_list();
void reprnt_save_file();

void atm_malloc(ATM *atm, ATM_METADATA *meta);
void atm_to_file(ATM *atm);
void atm_to_list(ATM *atm);
void update_atm_file(int atm_index, int data_type, char *modified_data);

void random_account(char *name, char *account, char *pin, char *balance);
int validate_created_data(char *data, int data_size, int data_type);

int account_input(char *input, int *input_size, char *ch);
int pin_input(char *input, int *input_size, char *ch, ATM *atm_to_check, int is_censored);
long long int money_input(char *input, int *input_size, char *ch, int mode);
void receipt(long long int withdraw_amount, int withdraw_mode);
void free_atm(ATM *atm);
void free_everything();

// Global variables

ATM *cur_atm_ptr, *atm_list = NULL;
size_t atm_list_size = 0, atm_list_buffer_size = 10;
int cur_atm_index = -1;

// Function definitions

void init_data_manager() {
    _mkdir("data");
    init_meta_manager();
    init_atm_list();
    reprnt_save_file();
}

void reprnt_save_file() {
    if(meta_cmp(&main_meta, &save_meta) == 0) {
        meta_to_file(&main_meta, SAVE_FILE);

        for(int i = 0; i < atm_list_size; i++) atm_to_file(&atm_list[i]);
    }

}

void init_atm_list() {
    if(meta_from_file(&save_meta, SAVE_FILE) == 1) {
        meta_to_file(&main_meta, SAVE_FILE);
        save_meta = main_meta;
    }
    FILE *file = fopen(SAVE_FILE, "r+");

    if(atm_list != NULL) {
        free(atm_list);
        atm_list_size = 0;
    }
    atm_list = malloc(atm_list_buffer_size * sizeof(ATM));
    ATM temp_atm;
    atm_malloc(&temp_atm, &save_meta);

    char format[100], line[150];
    sprintf(format, "%s%c%%%dc%c%s%c%%%ds%c%s%c%%%ds%c%s%c%%%dc%c%s%c%%%ds%c",
        save_meta.tags[0], save_meta.separator, save_meta.data_sizes[0], save_meta.separator,
        save_meta.tags[1], save_meta.separator, save_meta.data_sizes[1], save_meta.separator,
        save_meta.tags[2], save_meta.separator, save_meta.data_sizes[2], save_meta.separator,
        save_meta.tags[3], save_meta.separator, save_meta.data_sizes[3] + 2, save_meta.separator,
        save_meta.tags[4], save_meta.separator, save_meta.data_sizes[4], save_meta.separator
    );

    char balance_buffer[50], attempt_buffer[10];
    fscanf(file, "%[^\n]\n", line);
    while(fscanf(file, "%[^\n]\n", line) == 1) {
        sscanf(line, format, temp_atm.name, temp_atm.account, temp_atm.pin, balance_buffer, attempt_buffer);

        char *trim_ptr = temp_atm.name + save_meta.data_sizes[0] - 1;
        while(isspace(*--trim_ptr));
        *(trim_ptr + 1) = '\0';

        temp_atm.balance = strtoll(balance_buffer, NULL, 10);
        temp_atm.PIN_attempt = strtol(attempt_buffer, NULL, 10);

        atm_to_list(&temp_atm);
    }

    fclose(file);

    return;
}

void atm_malloc(ATM *atm, ATM_METADATA *meta) {
    atm->name = malloc((meta->data_sizes[0] + 1) * sizeof(char));
    atm->account = malloc((meta->data_sizes[1] + 1) * sizeof(char));
    atm->pin = malloc((meta->data_sizes[2] + 1) * sizeof(char));
    atm->balance = 0;
    atm->PIN_attempt = -1;
}

void atm_to_file(ATM *atm) {
    FILE *file = fopen(SAVE_FILE, "a");

    fprintf(file,
        "%s%c%-*s%c%s%c%*s%c%s%c%*s%c%s%c%-*lld%c%s%c%-*d%c\n",
        main_meta.tags[0], main_meta.separator, main_meta.data_sizes[0], atm->name, main_meta.separator,
        main_meta.tags[1], main_meta.separator, main_meta.data_sizes[1], atm->account, main_meta.separator,
        main_meta.tags[2], main_meta.separator, main_meta.data_sizes[2], atm->pin, main_meta.separator,
        main_meta.tags[3], main_meta.separator, main_meta.data_sizes[3] + 2, atm->balance, main_meta.separator,
        main_meta.tags[4], main_meta.separator, main_meta.data_sizes[4], atm->PIN_attempt, main_meta.separator
    );

    fclose(file);
}

void atm_to_list(ATM *atm) {
    atm_list[atm_list_size].name = strdup(atm->name);
    atm_list[atm_list_size].account = strdup(atm->account);
    atm_list[atm_list_size].pin = strdup(atm->pin);
    atm_list[atm_list_size].balance = atm->balance;
    atm_list[atm_list_size].PIN_attempt = atm->PIN_attempt;

    atm_list_size++;

    if(atm_list_size >= atm_list_buffer_size) {
        atm_list_buffer_size *= 2;
        atm_list = realloc(atm_list, atm_list_buffer_size * sizeof(ATM));
    }
}

/**
 * @brief   Change indexed account's one specific data in save file
 * 
 * @param atm_index     Account index
 * @param data_type     0: Name, 1: Account, 2: Pin, 3: Balance, 4: PIN attempts
 * @param modified_data String of value to change
 */
void update_atm_file(int atm_index, int data_type, char *modified_data) {
    FILE *file = fopen(SAVE_FILE, "r+");

    for(int i = 0; i < atm_index + 1; i++) fscanf(file, "%*[^\n]\n");

    fseek(file, 0, SEEK_CUR);
    fprintf(file,
        "%s%c%-*s%c%s%c%*s%c%s%c%*s%c%s%c%-*lld%c%s%c%-*d%c\n",
        main_meta.tags[0], main_meta.separator, main_meta.data_sizes[0], atm_list[atm_index].name, main_meta.separator,
        main_meta.tags[1], main_meta.separator, main_meta.data_sizes[1], atm_list[atm_index].account, main_meta.separator,
        main_meta.tags[2], main_meta.separator, main_meta.data_sizes[2], atm_list[atm_index].pin, main_meta.separator,
        main_meta.tags[3], main_meta.separator, main_meta.data_sizes[3] + 2, atm_list[atm_index].balance, main_meta.separator,
        main_meta.tags[4], main_meta.separator, main_meta.data_sizes[4], atm_list[atm_index].PIN_attempt, main_meta.separator
    );

    fclose(file);
}

void random_account(char *name, char *account, char *pin, char *balance) {
    static const char *last_names[] = {"NGUYEN", "TRAN", "LE", "HOANG", "PHAM", "DANG", "NGO"};
    static const char *middle_names[] = {"GIA", "TRA", "THANH", "VAN", "THI", "DUONG", "LINH"};
    static const char *first_names[] = {"ANH", "HA", "HOA", "LY", "GIANG", "KIM", "TU", "LINH"};

    int lname_rand = rand() % (sizeof(last_names) / sizeof(last_names[0]));
    int mname_rand = rand() % (sizeof(middle_names) / sizeof(middle_names[0]));
    int fname_rand = rand() % (sizeof(first_names) / sizeof(first_names[0]));

    sprintf(name, "%s %s %s", last_names[lname_rand], middle_names[mname_rand], first_names[fname_rand]);
    sprintf(account, "%04d%04d%04d%02d", rand() % 10000, rand() % 10000, rand() % 10000, rand() % 100);
    sprintf(pin, "%03d%03d", rand() % 1000, rand() % 1000);
    sprintf(balance, "%d00000", rand() % 10000);
}

/**
 * @param data_type    0: Name, 1: Account, 2: Pin, 3: Balance
 * 
 * @return  0: Invalid, 1: Valid, -1: Duped acc ('data_type' = 1)
 */
int validate_created_data(char *data, int data_size, int data_type) {
    if(data_type == 1) for(int i = 0; i < atm_list_size; i++) if(strcmp(data, atm_list[i].account) == 0) return -1;
    if(data_type == 3 && strtoll(data, NULL, 10) < main_meta.bal_create_min) return 0;
    return (data_type == 1 || data_type == 2) ? data_size == main_meta.data_sizes[data_type] : (data_size > 0 && data_size <= main_meta.data_sizes[data_type]);
}

/**
 * @brief   Keep taking input until valid account or ESC entered
 * 
 * @return  Account index or OP_CANCELLED if ESC
 */
int account_input(char *input, int *input_size, char *ch) {
    int acc_index = -1;
    char invalid_msg[100];
    while(1) {
        if((*input_size = unbuffered_input(input, main_meta.data_sizes[1], 1, 0, *ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(*input_size == main_meta.data_sizes[1]) {
            for(int i = 0; i < atm_list_size; i++) {
                if(strcmp(input, atm_list[i].account) == 0) {
                    acc_index = i;
                    break;
                }
            }
            if(acc_index != -1) break;

            prnt_invalid("Unregistered Account", *input_size, ch);
            continue;
        }
        
        sprintf(invalid_msg, "Invalid Account (Not %d Digits)", main_meta.data_sizes[1]);
        prnt_invalid(invalid_msg, *input_size, ch);
    }
    putchar('\n');
    return acc_index;
}

/**
 * @brief   Keep taking input until valid PIN or ESC entered
 * 
 * @param input             Allocated char* to store input
 * @param input_size        int* to store input size
 * @param ch                ch* to store first character entered
 * @param atm_to_check      If !NULL: used to compare entered PIN
 * @param is_censored       If 1: show entered PIN as '*'
 * 
 * @return  OP states: CANCELLED, FINISHED, FAILED
 */
int pin_input(char *input, int *input_size, char *ch, ATM *atm_to_check, int is_censored) {
    *ch = 0;

    char invalid_msg[100], attempts_str[10];
    while(1) {
        if(atm_to_check != NULL && atm_to_check->PIN_attempt == 0) {
            prnt_invalid("Account Is Locked, Please Contact Customer Service", 0, ch);
            return OP_FAILED;
        }
        if((*input_size = unbuffered_input(input, main_meta.data_sizes[2], 1, is_censored, *ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(*input_size == main_meta.data_sizes[2]) {
            if(atm_to_check != NULL) {
                if(strcmp(input, atm_to_check->pin) != 0) {
                    (atm_to_check->PIN_attempt)--;
                    if(atm_to_check->PIN_attempt < 5 && atm_to_check->PIN_attempt >= 0) sprintf(invalid_msg, "Incorrect PIN, %d Attempts Left", atm_to_check->PIN_attempt);
                    else sprintf(invalid_msg, "Incorrect PIN");
                    prnt_invalid(invalid_msg, *input_size, ch);

                    sprintf(attempts_str, "%d", atm_to_check->PIN_attempt);
                    update_atm_file(cur_atm_index, 4, attempts_str);
                    continue;
                }
                atm_to_check->PIN_attempt = DEFAULT_ATTEMPTS_AMOUNT;
                sprintf(attempts_str, "%d", DEFAULT_ATTEMPTS_AMOUNT);
                update_atm_file(cur_atm_index, 4, attempts_str);
            }
            break;
        }
        else {
            sprintf(invalid_msg, "Invalid PIN (Not %d Digits)", main_meta.data_sizes[2]);
            prnt_invalid(invalid_msg, *input_size, ch);
        }
    }
    putchar('\n');

    return OP_FINISHED;
}

/**
 * @brief   Withdraw money from cur_atm_ptr->balance, keep taking input until valid amount or ESC entered
 * 
 * @param mode  1: Withdrawing, 2: Transfering
 * 
 * @return  Withdraw amount, OP_CANCELLED if ESC
 */
long long int money_input(char *input, int *input_size, char *ch, int mode) {
    long long int withdraw_amount = 0;
    *ch = 0;

    char max_withdraw[100];
    str_to_money(max_withdraw, main_meta.bal_withdraw_max);
    char overwithdraw_msg[100] = "Must Be Under ";
    strcat(overwithdraw_msg, max_withdraw);

    char min_transfer[100];
    str_to_money(min_transfer, 10000);
    char undertransfer_msg[100] = "Must Be Over ";
    strcat(undertransfer_msg, min_transfer);

    switch(mode) {
        case 1: printf(" Maximum Withdraw Amount: %s\n", max_withdraw); break;
        case 2: printf(" Minimum Transfer Amount: %s\n", min_transfer); break;
    }
    
    printf(" Enter the Amount: ");
    while(1) {
        if((*input_size = unbuffered_input(input, main_meta.data_sizes[3], 1, 0, *ch)) == -1) return OP_CANCELLED;

        withdraw_amount = strtoll(input, NULL, 10);
        if(*input_size == 0 || withdraw_amount == 0) prnt_invalid("Invalid Ammount", *input_size, ch);
        else if(withdraw_amount > cur_atm_ptr->balance) prnt_invalid("Not Enough Money", *input_size, ch);
        else if(mode == 1 && withdraw_amount > main_meta.bal_withdraw_max) prnt_invalid(overwithdraw_msg, *input_size, ch);
        else if(mode == 1 && withdraw_amount % 100000 != 0) prnt_invalid("Must Be Multiple of 100.000", *input_size, ch);
        else if(mode == 2 && withdraw_amount < 10000) prnt_invalid(undertransfer_msg, *input_size, ch);
        else break;
    }
    putchar('\n');

    return withdraw_amount;
}

/**
 * @param withdraw_mode     1: Withdraw cash, 2: Transfer
 */
void receipt(long long int withdraw_amount, int withdraw_mode) {
    system("cls");
    _mkdir("receipts");

    char result_str[(UI_WIDTH + 1) * 18 + 1], line_write[UI_WIDTH + 2], buffer[101], buffer2[101];
    int pad;
    result_str[0] = '\0';

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    pad = (UI_WIDTH + strlen(main_meta.bank_name)) / 2;
    sprintf(line_write, "%*s%*s\n", pad, main_meta.bank_name, UI_WIDTH - pad, "");
    strcat(result_str, line_write);

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    sprintf(buffer, "BIEN LAI RUT TIEN TAI ATM");
    pad = (UI_WIDTH + strlen(buffer)) / 2;
    sprintf(line_write, "%*s%*s\n", pad, buffer, UI_WIDTH - pad, "");
    strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);
    
    strftime(buffer, 100, " Ngay: %x", tm_info);
    strftime(buffer2, 100, "Gio: %X", tm_info);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " So the:");
    strcpy(buffer2, cur_atm_ptr->account);
    memset(buffer2 + 4, '*', main_meta.data_sizes[1] - 8);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " So giao dich:");
    strftime(buffer2, 100, "%y%m%d%H%M%S", tm_info);
    sprintf(buffer2 + strlen(buffer2), "%d", rand() % 10000);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    char receipt_file_name[100];
    sprintf(receipt_file_name, "receipts\\receipt-%s.txt", buffer2);

    sprintf(buffer, " Noi dung:");
    switch(withdraw_mode) {
        case 1: sprintf(buffer2, "Rut tien mat tai ATM"); break;
        case 2: sprintf(buffer2, "Chuyen tien tai ATM"); break;
    }
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    sprintf(buffer, " So tien:");
    str_to_money(buffer2, withdraw_amount);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " Le phi:");
    str_to_money(buffer2, 1000);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    sprintf(buffer, " VAT:");
    str_to_money(buffer2, 100);
    sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    strcat(result_str, line_write);

    // sprintf(buffer, " So du:");
    // str_to_money(buffer2, cur_atm_ptr->balance);
    // sprintf(line_write, "%s%*s \n", buffer, UI_WIDTH - strlen(buffer) - 1, buffer2);
    // strcat(result_str, line_write);

    str_to_line(line_write, 0);
    strcat(result_str, line_write);

    sprintf(buffer, "Cam on quy khach da su dung"),
    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(buffer)) / 2, buffer);
    strcat(result_str, line_write);

    sprintf(buffer, "dich vu cua chung toi!");
    sprintf(line_write, "%*s\n", (UI_WIDTH + strlen(buffer)) / 2, buffer);
    strcat(result_str, line_write);

    str_to_line(line_write, 1);
    strcat(result_str, line_write);

    printf("%s", result_str);

    FILE *file = fopen(receipt_file_name, "w");
    fprintf(file, "%s", result_str);
    fclose(file);
}

void free_atm(ATM *atm) {
    free(atm->name);
    free(atm->account);
    free(atm->pin);
}

void free_everything() {
    for(int i = 0; i < atm_list_size; i++) free(atm_list + i);
    free(atm_list);
}

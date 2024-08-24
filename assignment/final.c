#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <math.h>

#define UI_WIDTH 50
#define UI_PROMPT_MSG_LEN 23

#define ATM_NAME_LEN 50
#define ATM_ACCOUNT_LEN 14
#define ATM_PIN_LEN 6
#define ATM_BALANCE_LEN 18
#define ATM_BALANCE_MIN_VALUE 50000
#define ATM_TOTAL_META_LEN ATM_NAME_LEN + ATM_ACCOUNT_LEN + ATM_PIN_LEN + ATM_BALANCE_LEN

typedef struct ATM
{
    char name[ATM_NAME_LEN + 1];
    char account[ATM_ACCOUNT_LEN + 1];
    char pin[ATM_PIN_LEN + 1];
    long long int balance;
} ATM;

const char *BANK_NAME = "VTC Academy Bank";
const char *FILE_NAME = "account-number.dat";

int prnt_welcome_scr();
int prnt_creating_scr();
int prnt_login_scr();

void prnt_ui_line(int double_line);
void prnt_line(size_t len, int double_line);
int unbuffered_input(char *target_buffer, size_t max_buffer_size, int mode, int is_censored, char first_ch);
int choice_input(int min, int max);

ATM *cur_atm, *atm_list;
size_t atm_list_size = 0, atm_list_buffer_size = 10;
int cur_atm_index;

int main() {
    srand(time(NULL));
    
    while(1) {
        switch(prnt_welcome_scr()) {
            case 1:
                while(prnt_creating_scr() != 0);
                continue;
                break;

            case 2:
                while(prnt_login_scr() != 0);
                continue;
                break;

            case 0:
                printf(" Ending program...");
                exit(0);
        }
    }
}

/**
 * @return 1: create acc, 2: login, 0: end
 */
int prnt_welcome_scr() {
    //system("cls");
    prnt_ui_line(1);

    char header[UI_WIDTH];
    sprintf(header, "Welcome to %s!", BANK_NAME);
    printf("%*s\n", (UI_WIDTH + strlen(header)) / 2, header);

    prnt_ui_line(1);

    printf(
        " 1. Create new account\n"
        " 2. Login\n"
        " 0. End program\n"
    );

    prnt_ui_line(0);

    return choice_input(0, 2);
}

int validate_data(char *data, int data_len, int data_std_len, int is_exact_len) {
    if(data_std_len == ATM_BALANCE_LEN && strtoll(data, NULL, 10) < ATM_BALANCE_MIN_VALUE) return 0;
    return is_exact_len ? strlen(data) == data_len : (strlen(data) > 0 && strlen(data) <= data_len);
}

void random_account(char *name, char *account, char *pin, char *balance) {
    static const char *last_names[] = {"Nguyen", "Tran", "Le", "Hoang", "Pham"};
    static const char *middle_names[] = {"Gia", "Tra", "Thanh", "Van", "Thi"};
    static const char *first_names[] = {"Anh", "Ha", "Linh", "Ly", "Duong"};

    int lname_rand = rand() % (sizeof(last_names) / sizeof(last_names[0]));
    int mname_rand = rand() % (sizeof(middle_names) / sizeof(middle_names[0]));
    int fname_rand = rand() % (sizeof(first_names) / sizeof(first_names[0]));

    sprintf(name, "%s %s %s", last_names[lname_rand], middle_names[mname_rand], first_names[fname_rand]);
    sprintf(account, "%04d%04d%04d%02d", rand() % 10000, rand() % 10000, rand() % 10000, rand() % 100);
    sprintf(pin, "%03d%03d", rand() % 1000, rand() % 1000);
    sprintf(balance, "%d00000", rand() % 1000);
}

/**
 * @return  1: continue making acc, 0: done making acc
 */
int prnt_creating_scr()
{
    system("cls");
    prnt_ui_line(1);
    printf("%*s\n", (UI_WIDTH + strlen(BANK_NAME)) / 2, BANK_NAME);
    prnt_ui_line(1);
    printf(
        " Creating New Account...\n"
        " Press ESC To Randomize.\n"
    );
    prnt_ui_line(0);

    char *prompts[2][4] = {
        {" Input Account Name:", " Input Account No:", " Input Pin:", " Input Balance:"},
        {"name", "account", "pin", "balance"}
    };
    int metadata[3][4] = {
        {ATM_NAME_LEN, ATM_ACCOUNT_LEN, ATM_PIN_LEN, ATM_BALANCE_LEN},
        {2, 1, 1, 1},   // Mode
        {0, 1, 1, 0}    // Is exact len
    };
    char atm_buffer[4][100] = {
        "Nguyen Van A"
    };
    random_account(atm_buffer[0], atm_buffer[1], atm_buffer[2], atm_buffer[3]);

    char input_buffer[100], invalid_msg[100], ch;
    int input_size;
    for(int i = 0; i < 4; i++) {
        ch = 0;

        printf("%-*s", UI_PROMPT_MSG_LEN, prompts[0][i]);
        while(1) {
            strcpy(input_buffer, atm_buffer[i]);
            input_size = unbuffered_input(input_buffer, metadata[0][i], metadata[1][i], 0, ch);

            if(input_size == -1) break;;
            if(validate_data(input_buffer, input_size, metadata[0][i], metadata[2][i])) break;

            while(input_size-- > 0) printf("\b \b");
            sprintf(invalid_msg, "Invalid %s", prompts[1][i]);
            printf("%s", invalid_msg);

            ch = _getch();
            for(int j = 0; j < strlen(invalid_msg); j++) printf("\b \b");
        }

        strcpy(atm_buffer[i], input_buffer);
        putchar('\n');
    }

    prnt_ui_line(0);
    printf(" Do you want to create ATM Card? (Y/N): ");
    if((ch = _getch()) == 'Y' || ch == 'y') {
        FILE *file = fopen(FILE_NAME, "a");

        ATM new_atm;
        strcpy(new_atm.name, atm_buffer[0]);
        strcpy(new_atm.account, atm_buffer[1]);
        strcpy(new_atm.pin, atm_buffer[2]);
        new_atm.balance = strtoll(atm_buffer[3], NULL, 10);

        fprintf(file,
            "NAME: %*s ACCOUNT: %*s PIN: %*s BALANCE: %*lld\n",
            ATM_NAME_LEN, new_atm.name,
            ATM_ACCOUNT_LEN, new_atm.account,
            ATM_PIN_LEN, new_atm.pin,
            ATM_BALANCE_LEN, new_atm.balance
        );

        fclose(file);
    }
    putchar('\n');
    
    prnt_ui_line(0);
    printf(" Do you want to create another account? (Y/N): ");
    if((ch = _getch()) == 'Y' || ch == 'y') return 1;
    putchar('\n');

    prnt_ui_line(0);
    printf(" Press any key to return...\n");
    _getch();
    return 0;
}

int prnt_login_scr()
{
    return 0;
}

void prnt_ui_line(int double_line) {
    prnt_line(UI_WIDTH, double_line);
}

void prnt_line(size_t len, int double_line) {
    static char ch;
    ch = double_line ? '=' : '-';
    while(len-- > 0) putchar(ch);
    putchar('\n');
}

/**
 * @brief   Take unbuffered input (instantly as user enters any key)
 * 
 * @param target_buffer     An allocated char* to store input
 * @param max_buffer_size   Maximum amount of characters to take from input
 * @param mode              type of input, 1: only 0-9, 2: only A-z, 3: both
 * @param is_censored       1: show entered character as '*'
 * @param first_ch          If !0, take this as first char instead
 * 
 * @return  Size of input, -1 if esc
 */
int unbuffered_input(char *target_buffer, size_t max_buffer_size, int mode, int is_censored, char first_ch) {
    size_t buffer_size = 0;
    char ch, buffer[max_buffer_size + 1];

    while(1) {
        if(first_ch != 0) {
            ch = first_ch;
            first_ch = 0;
        } else ch = _getch();

        if(ch == '\n' || ch == '\r') break;
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

/**
 * @brief   Returns when input meets the condition, otherwise keep taking input. Prints "Your choice: " itself
 */
int choice_input(int min, int max) {
    char input_buffer[2] = "\0", ch = 0;
    int choice, input_len;

    printf(" Your Choice: ");
    while (1) {
        input_len = unbuffered_input(input_buffer, 1, 1, 0, ch);
        choice = input_len > 0 ? strtol(input_buffer, NULL, 10) : -1;

        if(choice >= min && choice <= max) break;

        printf("%sInvalid Choice", input_len > 0 ? "\b" : "");
        ch = _getch();
        for(int i = 0; i < strlen("Invalid Choice"); i++) printf("\b \b");
    }
    putchar('\n');

    return choice;
}

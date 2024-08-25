#include "utility.c"

int welcome_scr();
int creating_scr();
int login_scr();
int acc_mng_scr();
int withdraw_scr();
int transfer_scr();
int pin_chng_scr();

// Operation states
#define OP_LOOP        1
#define OP_FINISH      0
#define OP_CANCELLED   -1

int main() {
    srand(time(NULL));

    init_atm_list();
    
    while(1) {
        switch(welcome_scr()) {
            case 1:
                while(creating_scr() == OP_LOOP);
                continue;

            case 2:
                switch(login_scr()) {
                    case OP_FINISH: while(acc_mng_scr() == OP_LOOP);
                    case OP_CANCELLED: continue;
                }

            case 0:
                printf(" Ending program...");
                exit(0);
        }
    }

    free_everything();
    return 0;
}

/**
 * @return  1: Create acc, 2: Login, 0: End
 */
int welcome_scr() {
    system("cls");
    char header[UI_WIDTH];
    sprintf(header, "Welcome to %s!", BANK_NAME);

    prnt_ui_line(1);
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

/**
 * @return  OP states: 1, 0
 */
int creating_scr() {
    system("cls");
    prnt_header();
    printf(
        " Creating New Account...\n"
        " Press ESC to Randomize.\n"
    );
    prnt_ui_line(0);

    char *prompts[2][4] = {
        {" Enter Account Name:", " Enter Account No:", " Enter Pin:", " Enter Balance:"},
        {"name", "account", "pin", "balance"}
    };
    int metadata[2][4] = {
        {2, 1, 1, 1},   // Input mode
        {0, 1, 1, 0}    // Is exact len
    };
    char atm_buffer[4][100], input[100], invalid_msg[100], ch;
    int input_size, changed;

    random_account(atm_buffer[0], atm_buffer[1], atm_buffer[2], atm_buffer[3]);

    for(int i = 0; i < 4; i++) {
        ch = 0;
        changed = 1;
        printf("%-*s", UI_PROMPT_MSG_LEN, prompts[0][i]);

        while(1) {
            strcpy(input, atm_buffer[i]);

            if((input_size = unbuffered_input(input, DATA_LEN[i], metadata[0][i], 0, ch)) == OP_CANCELLED) {
                printf("%s", atm_buffer[i]);
                changed = 0;
                break;
            }

            int valid = validate_data(input, input_size, i);
            if(valid == 1) break;
            if(valid == 0) sprintf(invalid_msg, "Invalid %s", prompts[1][i]);
            if(valid == -1) sprintf(invalid_msg, "Duplicated %s", prompts[1][i]);

            prnt_invalid(invalid_msg, input_size, &ch);
        }

        if(changed) strcpy(atm_buffer[i], input);
        putchar('\n');
    }
    standardize_str(atm_buffer[0]);

    prnt_ui_line(0);
    printf(" Do you want to create ATM Card? (Y/N): ");
    if(yes_no_input()) {
        ATM new_atm;
        strcpy(new_atm.name, atm_buffer[0]);
        strcpy(new_atm.account, atm_buffer[1]);
        strcpy(new_atm.pin, atm_buffer[2]);
        new_atm.balance = strtoll(atm_buffer[3], NULL, 10);

        atm_to_file(&new_atm);
        atm_to_list(&new_atm);

        printf(" Created ATM Card successfully!\n");
    }

    prnt_ui_line(0);
    printf(" Press ESC to return, any other key to continue making accounts.");
    if(_getch() == 27) return OP_FINISH;
    return OP_LOOP;
}

/**
 * @return  -1: Cancelled, 1: Successfully logged in
 */
int login_scr() {
    system("cls");
    prnt_header();
    printf(" Logging In...\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    char input[100], prompt[UI_PROMPT_MSG_LEN + 1], ch = 0;
    int input_size;

    sprintf(prompt, " Enter Account No:");
    printf("%-*s", UI_PROMPT_MSG_LEN, prompt);
    while(1) {
        if((input_size = unbuffered_input(input, DATA_LEN[1], 1, 0, ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(input_size == DATA_LEN[1]) {
            for(int i = 0; i < atm_list_size; i++) {
                if(strcmp(input, atm_list[i].account) == 0) {
                    cur_index = i;
                    cur_atm = atm_list + i;
                    break;
                }
            }
            if(cur_index != -1) break;

            prnt_invalid("Unregistered account", input_size, &ch);
            continue;
        }
        
        prnt_invalid("Invalid account", input_size, &ch);
    }
    putchar('\n');

    ch = 0;
    sprintf(prompt, " Enter PIN:");
    printf("%-*s", UI_PROMPT_MSG_LEN, prompt);
    if(enter_pin(input, &input_size, &ch, cur_atm->pin, 1) == OP_CANCELLED) return OP_CANCELLED;

    prnt_ui_line(0);
    printf(" Greetings, %s!\n Press any key to continue...", cur_atm->name);
    _getch();
    return OP_FINISH;
}

/**
 * @return  -1: Cancelled, 0: Done, 1: Continue
 */
int acc_mng_scr() {
    char money_str[DATA_LEN[3] + 10];

    system("cls");
    prnt_header();
    printf(" Account Name:   %s\n Account Number: %s\n", cur_atm->name, cur_atm->account);
    prnt_ui_line(0);

    printf(
        " 1. Check Balance\n"
        " 2. Withdraw Cash\n"
        " 3. Transfer Money\n"
        " 4. Change PIN\n"
        " 0. Log Out\n"
    );
    prnt_ui_line(0);
    
    switch(choice_input(0, 4)) {
        case 1: 
            prnt_ui_line(0);
            money_to_str(money_str, cur_atm->balance);
            printf(" Current Balance: %s\n Press any key to continue...", money_str);
            _getch();
            break;
        case 2: while(withdraw_scr() == OP_LOOP); break;
        case 3: while(transfer_scr() == OP_LOOP); break;
        case 4: pin_chng_scr(); break;
        case 0:
            prnt_ui_line(0);
            printf(" Logging out...\n Press any key to continue...");
            _getch();
            return OP_FINISH;
    }

    return OP_LOOP;
}

/**
 * @return  -1: Cancelled, 0: Done, 1: Continue
 */
int withdraw_scr() {
    long long int withdraw_amount = 0;
    char input[100], ch = 0, money_str[DATA_LEN[3] + 10];
    int input_size;

    system("cls");
    prnt_header();
    printf(" Withdrawing Cash...\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    money_to_str(money_str, cur_atm->balance);
    printf(" Account Balance: %s\n", money_str);
    prnt_ui_line(0);
    
    printf(
        " 1.   100.000 VND\n"
        " 2.   200.000 VND\n"
        " 3.   500.000 VND\n"
        " 4. 1.000.000 VND\n"
        " 5. 2.000.000 VND\n"
        " 6. Other amount\n"
    );
    prnt_ui_line(0);

    switch(choice_input(1, 6)) {
        case 1: withdraw_amount = 100000; break;
        case 2: withdraw_amount = 200000; break;
        case 3: withdraw_amount = 500000; break;
        case 4: withdraw_amount = 1000000; break;
        case 5: withdraw_amount = 2000000; break;
        case 6:
            if((withdraw_amount = get_money(input, &input_size, &ch)) == OP_CANCELLED) return OP_CANCELLED;
            break;
        case -1: return OP_CANCELLED;
    }

    money_to_str(money_str, withdraw_amount);
    printf(" Withdraw amount: %s\n Please confirm. (Y/N): ", money_str);

    if(yes_no_input()) {
        printf(" Withdrawed successfully %s!\n", money_str);
        cur_atm->balance -= withdraw_amount;
        money_to_str(money_str, cur_atm->balance);
        printf(" Remaining balance: %s\n", money_str);

        prnt_ui_line(0);
        printf(" Do you want to print receipt? (Y/N): ");
        if(yes_no_input()) {
            cur_atm->balance -= 1100;
            receipt(withdraw_amount);
        }

        sprintf(money_str, "%-*lld", DATA_LEN[3], cur_atm->balance);
        modify_file(cur_index, 3, money_str);
    }
    else printf(" Cancelled transaction.\n");

    prnt_ui_line(0);
    printf(" Press ESC to return, any other key to continue withdrawing.");
    if(_getch() == 27) return OP_FINISH;
    return OP_LOOP;
}

/**
 * @return  -1: Cancelled, 0: Done, 1: Continue
 */
int transfer_scr() {
    long long int transfer_amount = 0;
    char input[100], ch = 0, money_str[DATA_LEN[3] + 10];
    int input_size, target_index = -1;
    ATM *target_atm;

    system("cls");
    prnt_header();
    printf(" Transfering Money...\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    money_to_str(money_str, cur_atm->balance);
    printf(" Current Balance: %s\n", money_str);
    prnt_ui_line(0);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter Account No:");
    while(1) {
        if((input_size = unbuffered_input(input, DATA_LEN[1], 1, 0, ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(input_size == DATA_LEN[1]) {
            for(int i = 0; i < atm_list_size; i++) {
                if(strcmp(input, atm_list[i].account) == 0 && strcmp(input, cur_atm->account) != 0) {
                    target_index = i;
                    target_atm = atm_list + i;
                    break;
                }
            }
            if(target_index != -1) break;
        }
        
        // for(int i = 0; i < strlen(input); i++) printf("\b \b");
        prnt_invalid("Invalid account", input_size, &ch);
    }
    putchar('\n');
    printf(
        "%-*s%s\n"
        " Please Confirm Target Account. (Y/N): ",
        UI_PROMPT_MSG_LEN, " Account Name:", target_atm->name
    );
    if(!yes_no_input()) return OP_LOOP;

    if((transfer_amount = get_money(input, &input_size, &ch)) == OP_CANCELLED) return OP_CANCELLED;
    prnt_ui_line(0);

    money_to_str(money_str, transfer_amount);
    printf(" Transfer amount: %s\n Please confirm. (Y/N): ", money_str);

    if(yes_no_input()) {
        printf(" Transferred successfully %s!\n", money_str);
        cur_atm->balance -= transfer_amount;
        target_atm->balance += transfer_amount;

        sprintf(money_str, "%-*lld", DATA_LEN[3], cur_atm->balance);
        modify_file(cur_index, 3, money_str);
        sprintf(money_str, "%-*lld", DATA_LEN[3], target_atm->balance);
        modify_file(target_index, 3, money_str);
    }
    else printf(" Cancelled transaction.\n");

    prnt_ui_line(0);
    printf(" Press ESC to return, any other key to continue withdrawing.");
    if(_getch() == 27) return OP_FINISH;
    return OP_LOOP;
}

/**
 * @return  -1: Cancelled, 0: Done
 */
int pin_chng_scr() {
    char new_pin[DATA_LEN[2] + 1], input[100], ch;
    int input_size = 0;

    system("cls");
    prnt_header();
    printf(" Changing PIN...\n Press ESC to cancel.\n");
    prnt_ui_line(0);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter Current PIN:");
    if(enter_pin(input, &input_size, &ch, cur_atm->pin, 1) == OP_CANCELLED) return OP_CANCELLED;

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter New PIN:");
    if(enter_pin(input, &input_size, &ch, NULL, 0) == OP_CANCELLED) return OP_CANCELLED;
    strcpy(new_pin, input);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Re-enter New PIN:");
    if(enter_pin(input, &input_size, &ch, new_pin, 0) == OP_CANCELLED) return OP_CANCELLED;
    strcpy(cur_atm->pin, new_pin);
    modify_file(cur_index, 2, cur_atm->pin);

    prnt_ui_line(0);
    printf(" Changed PIN successfully!\n Press any key to return...");
    _getch();

    return OP_FINISH;
}

#include <windows.h>

#include "manager_atm_data.c"

/* Can only run on Windows due to <conio.h> & <windows.h> header */

int welcome_scr();
int creating_scr();
int login_scr();
int acc_mng_scr();
int withdraw_scr();
int transfer_scr();
int pin_chng_scr();

int main() {
    // Enable ANSI escape codes in cmd
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    srand(time(NULL));

    init_data_manager();
    
    int op_state;
    while(1) {
        switch(welcome_scr()) {
            case 1:
                while((op_state = creating_scr()) == OP_LOOPING);
                break;

            case 2:
                while((op_state = login_scr()) == OP_LOOPING);
                switch(op_state) {
                    case OP_FINISHED:
                        while(acc_mng_scr() == OP_LOOPING);
                    case OP_CANCELLED: case OP_FAILED:
                        break;
                }
                break;

            case 3:
                init_data_manager();
                break;

            case 0:
                printf(" Ending program...");
                exit(0);
                break;
        }
    }

    free_everything();
    return 0;
}

/**
 * @return  1: Create acc, 2: Login, 3: Reload, 0: End
 */
int welcome_scr() {
    // system("cls");
    printf("\033[1;1H\033[J");
    char header[] = "Welcome to ";
    int pad = (UI_WIDTH - strlen(header) - strlen(main_meta.bank_name) - 1) / 2;

    prnt_ui_line(1);
    printf("%*s\033[1m%s%s!\033[0m\n",  pad, "", header, main_meta.bank_name);
    prnt_ui_line(1);

    printf(
        " 1. Create New Account\n"
        " 2. Log In\n"
        " 3. Reload Save File\n"
        " 0. End Program\n"
    );
    prnt_ui_line(0);

    return choice_input(0, 3);
}

/**
 * @return  OP states: CANCELLED, LOOPING
 */
int creating_scr() {
    printf("\033[1;1H\033[J");
    prnt_header();
    printf(
        " Creating New Account...\n"
        " Press ESC to Randomize.\n"
    );
    prnt_ui_line(0);

    char *data_tags[4] = {"Account Name", "Account Number", "PIN", "Balance"};
    int exact_size[4] = {0, 1, 1, 0};
    int input_mode[4] = {2, 1, 1, 1};

    char atm_buffer[5][BUFFER_SIZE], input[BUFFER_SIZE], prompt_msg[BUFFER_SIZE], invalid_msg[BUFFER_SIZE], min_bal[BUFFER_SIZE], ch;
    int input_size, changed, valid;

    str_to_money(min_bal, main_meta.bal_create_min);
    random_account(atm_buffer[0], atm_buffer[1], atm_buffer[2], atm_buffer[3]);

    for(int i = 0; i < 4; i++) {
        ch = 0;
        changed = 1;
        sprintf(prompt_msg, " Enter %s", data_tags[i]);
        printf("%-*s: ", UI_PROMPT_MSG_LEN, prompt_msg);

        while(1) {
            strcpy(input, atm_buffer[i]);

            if((input_size = unbuffered_input(input, main_meta.data_sizes[i], exact_size[i], input_mode[i], 0, ch)) == OP_CANCELLED) {
                printf("%s", atm_buffer[i]);
                changed = 0;
                break;
            }

            if((valid = validate_created_data(input, input_size, i)) == 1) break;
            switch(valid) {
                case 0:
                    if(i == 1 || i == 2) sprintf(invalid_msg, "Invalid %s (Not %d Digits)", data_tags[i], main_meta.data_sizes[i]);
                    else sprintf(invalid_msg, "Invalid %s", data_tags[i]);
                    break;
                case 2: sprintf(invalid_msg, "Duplicated %s", data_tags[i]); break;
                case 3: sprintf(invalid_msg, "Must Be Over %s", min_bal); break;
            }
            prnt_invalid(invalid_msg, input_size, &ch);
        }

        if(changed) strcpy(atm_buffer[i], input);
        putchar('\n');
    }
    standardize_str(atm_buffer[0]);

    prnt_ui_line(0);
    printf(" Create ATM Card? (Y/N): ");
    if(yes_no_input()) {
        ATM new_atm;
        new_atm.name = strdup(atm_buffer[0]);
        new_atm.account = strdup(atm_buffer[1]);
        new_atm.pin = strdup(atm_buffer[2]);
        new_atm.balance = strtoll(atm_buffer[3], NULL, 10);
        new_atm.PIN_attempt = DEFAULT_ATTEMPTS_AMOUNT;

        atm_to_file(&new_atm);
        atm_to_list(&new_atm);

        printf(" Successfully Created ATM Card!\n");
    }

    prnt_ui_line(0);
    printf("\n Press ESC to return, any other key to continue creating ATM Card...");
    if(_getch() == 27) return OP_FINISHED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, LOOPING, FINISHED
 */
int login_scr() {
    printf("\033[1;1H\033[J");
    prnt_header();
    printf(" Logging In...\n\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    char input[BUFFER_SIZE], prompt[UI_PROMPT_MSG_LEN + 1], ch = 0;
    int input_size;

    sprintf(prompt, " Enter Account Number");
    printf("%-*s: ", UI_PROMPT_MSG_LEN, prompt);
    if((cur_atm_index = account_input(input, &input_size, &ch)) == OP_CANCELLED) return OP_CANCELLED;
    cur_atm_ptr = atm_list + cur_atm_index;

    ch = 0;
    sprintf(prompt, " Enter PIN");
    printf("%-*s: ", UI_PROMPT_MSG_LEN, prompt);
    switch(pin_input(input, &input_size, &ch, cur_atm_ptr, 1)) {
        case OP_CANCELLED: return OP_CANCELLED;
        case OP_FAILED: return OP_LOOPING;
    }

    prnt_ui_line(0);
    printf(" \033[32mGreetings, \033[0m%s\033[32m!\033[0m\n\n Press any key to continue...", cur_atm_ptr->name);
    _getch();
    return OP_FINISHED;
}

/**
 * @return  Op states: LOOPING, FINISHED, FAILED
 */
int acc_mng_scr() {
    char money_str[BUFFER_SIZE];

    printf("\033[1;1H\033[J");
    prnt_header();
    printf(" Account Name  : %s\n Account Number: %s\n", cur_atm_ptr->name, cur_atm_ptr->account);
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
            str_to_money(money_str, cur_atm_ptr->balance);
            printf(" Current Balance: %s\n\n Press any key to continue...", money_str);
            _getch();
            break;
        case 2: while(withdraw_scr() == OP_LOOPING); break;
        case 3: while(transfer_scr() == OP_LOOPING); break;
        case 4: while(pin_chng_scr() == OP_LOOPING); break;
        case 0:
            prnt_ui_line(0);
            printf(" \033[32mGoodbye, \033[0m%s\033[32m!\033[0m\n\n Press any key to continue...", cur_atm_ptr->name);
            _getch();
            return OP_FINISHED;
    }

    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED , LOOPING, FINISHED
 */
int withdraw_scr() {
    long long int withdraw_amount = 0;
    char input[BUFFER_SIZE], ch = 0, money_str[BUFFER_SIZE];
    int input_size;

    printf("\033[1;1H\033[J");
    prnt_header();
    printf(" Withdrawing Cash...\n\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    str_to_money(money_str, cur_atm_ptr->balance);
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
            if((withdraw_amount = money_input(input, &input_size, &ch, 1)) == OP_CANCELLED) return OP_CANCELLED;
            break;
        case OP_CANCELLED: return OP_CANCELLED;
    }
    if(withdraw_amount > cur_atm_ptr->balance) {
        printf(" \033[31mNot Enough Money In Balance...\033[0m");
        _getch();
        return OP_LOOPING;
    }

    str_to_money(money_str, withdraw_amount);
    printf("\n Withdraw Amount: %s\n Please Confirm (Y/N): ", money_str);

    if(yes_no_input()) {
        printf("\n Successfully Withdrawed %s!\n", money_str);
        cur_atm_ptr->balance -= withdraw_amount;
        str_to_money(money_str, cur_atm_ptr->balance);
        printf(" Remaining Balance: %s\n", money_str);

        prnt_ui_line(0);
        printf(" Print Receipt? (Y/N): ");
        if(yes_no_input()) {
            cur_atm_ptr->balance -= 1100;
            receipt(withdraw_amount, 1);
        }

        sprintf(money_str, "%-*lld", main_meta.data_sizes[3], cur_atm_ptr->balance);
        update_atm_file(cur_atm_index);
    }
    else {
        prnt_ui_line(0);
        printf(" \033[31mTransaction Cancelled.\033[0m\n");
    }

    printf("\n Press ESC to return, any other key to continue withdrawing...");
    if(_getch() == ESC_KEY) return OP_FINISHED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, LOOPING, FINISHED
 */
int transfer_scr() {
    long long int transfer_amount = 0;
    char input[BUFFER_SIZE], ch = 0, money_str[BUFFER_SIZE];
    int input_size, target_index = -1;
    ATM *target_atm;

    printf("\033[1;1H\033[J");
    prnt_header();
    printf(" Transfering Money...\n\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    str_to_money(money_str, cur_atm_ptr->balance);
    printf(" Current Balance: %s\n", money_str);
    prnt_ui_line(0);

    printf("%-*s: ", UI_PROMPT_MSG_LEN, " Enter Account Number");
    if((target_index = account_input(input, &input_size, &ch)) == OP_CANCELLED) return OP_CANCELLED;
    if(target_index == cur_atm_index) {
        printf("\033[31m Can't Transfer To Self.\033[0m\n");
        _getch();
        return OP_LOOPING;
    }
    target_atm = atm_list + target_index;
    
    printf(
        "%-*s: %s\n"
        " Please Confirm Beneficiary Account: (Y/N): ",
        UI_PROMPT_MSG_LEN, " Account Name", target_atm->name
    );
    if(!yes_no_input()) return OP_LOOPING;

    prnt_ui_line(0);
    if((transfer_amount = money_input(input, &input_size, &ch, 2)) == OP_CANCELLED) return OP_CANCELLED;

    str_to_money(money_str, transfer_amount);
    printf("\n Transfer Amount: %s\n Please Confirm (Y/N): ", money_str);

    if(yes_no_input()) {
        printf("\n Successfully Transferred %s!\n", money_str);
        cur_atm_ptr->balance -= transfer_amount;
        target_atm->balance += transfer_amount;
        
        str_to_money(money_str, cur_atm_ptr->balance);
        printf(" Remaining Balance: %s\n", money_str);

        prnt_ui_line(0);
        printf(" Print Receipt? (Y/N): ");
        if(yes_no_input()) {
            cur_atm_ptr->balance -= 1100;
            receipt(transfer_amount, 2);
        }

        sprintf(money_str, "%-*lld", main_meta.data_sizes[3], cur_atm_ptr->balance);
        update_atm_file(cur_atm_index);
        sprintf(money_str, "%-*lld", main_meta.data_sizes[3], target_atm->balance);
        update_atm_file(target_index);
    }
    else {
        prnt_ui_line(0);
        printf(" \033[31mTransaction Cancelled.\033[0m\n");
    }

    printf("\n Press ESC to return, any other key to continue transferring...");
    if(_getch() == ESC_KEY) return OP_FINISHED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, LOOPING, FINISHED
 */
int pin_chng_scr() {
    char input[BUFFER_SIZE], ch;
    int input_size = 0;
    ATM temp_atm;

    printf("\033[1;1H\033[J");
    prnt_header();
    printf(" Changing PIN...\n\n Press ESC to cancel.\n");
    prnt_ui_line(0);

    // printf("%-*s: ", UI_PROMPT_MSG_LEN, " Enter Current PIN");
    // switch(pin_input(input, &input_size, &ch, cur_atm_ptr, 1)) {
    //     case OP_CANCELLED: return OP_CANCELLED;
    //     case OP_FAILED: return OP_FAILED;
    // }

    printf("%-*s: ", UI_PROMPT_MSG_LEN, " Enter New PIN");
    if(pin_input(input, &input_size, &ch, NULL, 0) == OP_CANCELLED) return OP_CANCELLED;
    if(strcmp(input, cur_atm_ptr->pin) == 0) {
        printf("\033[31m Please Enter A New PIN.\033[0m\n");
        _getch();
        return OP_LOOPING;
    }

    atm_malloc(&temp_atm, &main_meta);
    strcpy(temp_atm.pin, input);

    printf("%-*s: ", UI_PROMPT_MSG_LEN, " Re-enter New PIN");
    if(pin_input(input, &input_size, &ch, &temp_atm, 0) == OP_CANCELLED) return OP_CANCELLED;
    strcpy(cur_atm_ptr->pin, temp_atm.pin);
    update_atm_file(cur_atm_index);

    prnt_ui_line(0);
    printf(" Successfully Changed PIN !\n\n Press any key to return...");
    _getch();

    return OP_FINISHED;
}

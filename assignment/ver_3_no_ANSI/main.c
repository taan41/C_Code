#include "data_manager.c"

int welcome_scr();
int creating_scr();
int login_scr();
int acc_mng_scr();
int withdraw_scr();
int transfer_scr();
int pin_chng_scr();

int main() {
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
    system("cls");
    
    char header[] = "Welcome to ";
    int pad = (UI_WIDTH - strlen(header) - strlen(main_meta.bank_name) - 1) / 2;

    prnt_ui_line(1);
    printf("%*s%s%s!\n",  pad, "", header, main_meta.bank_name);
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
    system("cls");
    prnt_header();
    printf(
        " Creating New Account...\n"
        " Press ESC to Randomize.\n"
    );
    prnt_ui_line(0);

    char *data_tags[4] = {"Account Name", "Account", "PIN", "Balance"};
    int input_mode[4] = {2, 1, 1, 1};

    char atm_buffer[5][100], input[100], prompt_msg[100], invalid_msg[100], ch;
    int input_size, changed;

    random_account(atm_buffer[0], atm_buffer[1], atm_buffer[2], atm_buffer[3]);

    for(int i = 0; i < 4; i++) {
        ch = 0;
        changed = 1;

        if(i == 1 || i == 2) sprintf(prompt_msg, " Enter %s (%d digits):", data_tags[i], main_meta.data_sizes[i]);
        else sprintf(prompt_msg, " Enter %s: ", data_tags[i]);
        printf("%-*s", UI_PROMPT_MSG_LEN + 7, prompt_msg);

        while(1) {
            strcpy(input, atm_buffer[i]);

            if((input_size = unbuffered_input(input, main_meta.data_sizes[i], input_mode[i], 0, ch)) == OP_CANCELLED) {
                printf("%s", atm_buffer[i]);
                changed = 0;
                break;
            }

            int valid = validate_created_data(input, input_size, i);
            if(valid == 1) break;
            if(valid == 0) sprintf(invalid_msg, "Invalid %s", data_tags[i]);
            if(valid == -1) sprintf(invalid_msg, "Duplicated %s", data_tags[i]);

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
    printf("\n Press ESC to return, any other key to continue creating ATM Card...\n");
    if(_getch() == ESC_KEY) return OP_CANCELLED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, LOOPING, FINISHED
 */
int login_scr() {
    system("cls");
    prnt_header();
    printf(" Logging In...\n\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    char input[100], prompt[UI_PROMPT_MSG_LEN + 1], ch = 0;
    int input_size;

    sprintf(prompt, " Enter Account No:");
    printf("%-*s", UI_PROMPT_MSG_LEN, prompt);
    cur_index = -1;
    while(1) {
        if((input_size = unbuffered_input(input, main_meta.data_sizes[1], 1, 0, ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(input_size == main_meta.data_sizes[1]) {
            for(int i = 0; i < atm_list_size; i++) {
                if(strcmp(input, atm_list[i].account) == 0) {
                    cur_index = i;
                    cur_atm_ptr = &atm_list[i];
                    break;
                }
            }
            if(cur_index != -1) break;

            prnt_invalid("Unregistered Account", input_size, &ch);
            continue;
        }
        
        prnt_invalid("Invalid Account", input_size, &ch);
    }
    putchar('\n');

    ch = 0;
    sprintf(prompt, " Enter PIN:");
    printf("%-*s", UI_PROMPT_MSG_LEN, prompt);
    switch(pin_input(input, &input_size, &ch, cur_atm_ptr, 1)) {
        case OP_CANCELLED: return OP_CANCELLED;
        case OP_FAILED: return OP_LOOPING;
    }

    prnt_ui_line(0);
    printf(" Greetings, %s!\n\n Press any key to continue...\n", cur_atm_ptr->name);
    _getch();
    return OP_FINISHED;
}

/**
 * @return  OP states: LOOPING, FINISHED, FAILED
 */
int acc_mng_scr() {
    char money_str[main_meta.data_sizes[3] + 10];

    system("cls");
    prnt_header();
    printf(" Account Name:   %s\n Account Number: %s\n", cur_atm_ptr->name, cur_atm_ptr->account);
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
            printf(" Current Balance: %s\n\n Press any key to continue...\n", money_str);
            _getch();
            break;
        case 2: while(withdraw_scr() == OP_LOOPING); break;
        case 3: while(transfer_scr() == OP_LOOPING); break;
        case 4: if(pin_chng_scr() == OP_FAILED) return OP_FAILED; break;
        case 0:
            prnt_ui_line(0);
            printf(" Goodbye, %s!\n\n Press any key to continue...\n", cur_atm_ptr->name);
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
    char input[100], ch = 0, money_str[100];
    int input_size;

    system("cls");
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
        printf(" Not Enough Money In Balance...");
        _getch();
        return OP_LOOPING;
    }

    str_to_money(money_str, withdraw_amount);
    printf("\n Withdraw amount: %s\n Please confirm. (Y/N): ", money_str);

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
        update_atm_file(cur_index, 3, money_str);
    }
    else {
        prnt_ui_line(0);
        printf(" Transaction Cancelled.\n");
    }

    printf("\n Press ESC to return, any other key to continue withdrawing...\n");
    if(_getch() == ESC_KEY) return OP_FINISHED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, LOOPING, FINISHED
 */
int transfer_scr() {
    long long int transfer_amount = 0;
    char input[100], ch = 0, money_str[main_meta.data_sizes[3] + 10];
    int input_size, target_index = -1;
    ATM *target_atm;

    system("cls");
    prnt_header();
    printf(" Transfering Money...\n\n Press ESC to Cancel.\n");
    prnt_ui_line(0);

    str_to_money(money_str, cur_atm_ptr->balance);
    printf(" Current Balance: %s\n", money_str);
    prnt_ui_line(0);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter Account No:");
    while(1) {
        if((input_size = unbuffered_input(input, main_meta.data_sizes[1], 1, 0, ch)) == OP_CANCELLED) return OP_CANCELLED;
        if(input_size == main_meta.data_sizes[1]) {
            for(int i = 0; i < atm_list_size; i++) {
                if(strcmp(input, atm_list[i].account) == 0 && strcmp(input, cur_atm_ptr->account) != 0) {
                    target_index = i;
                    target_atm = atm_list + i;
                    break;
                }
            }
            if(target_index != -1) break;
        }
        
        prnt_invalid("Invalid Account", input_size, &ch);
    }
    putchar('\n');

    printf(
        "%-*s%s\n"
        " Please Confirm Beneficiary Account: (Y/N): ",
        UI_PROMPT_MSG_LEN, " Account Name:", target_atm->name
    );
    if(!yes_no_input()) return OP_LOOPING;

    prnt_ui_line(0);
    if((transfer_amount = money_input(input, &input_size, &ch, 2)) == OP_CANCELLED) return OP_CANCELLED;

    str_to_money(money_str, transfer_amount);
    printf("\n Transfer Amount: %s\n Please Confirm. (Y/N): ", money_str);

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
        update_atm_file(cur_index, 3, money_str);
        sprintf(money_str, "%-*lld", main_meta.data_sizes[3], target_atm->balance);
        update_atm_file(target_index, 3, money_str);
    }
    else {
        prnt_ui_line(0);
        printf(" mTransaction Cancelled.\n");
    }

    printf("\n Press ESC to return, any other key to continue transferring...\n");
    if(_getch() == ESC_KEY) return OP_FINISHED;
    return OP_LOOPING;
}

/**
 * @return  OP states: CANCELLED, FINISHED, FAILED
 */
int pin_chng_scr() {
    char input[100], ch;
    int input_size = 0;
    ATM temp_atm;
    atm_malloc(&temp_atm, &main_meta);

    system("cls");
    prnt_header();
    printf(" Changing PIN...\n\n Press ESC to cancel.\n");
    prnt_ui_line(0);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter Current PIN:");
    switch(pin_input(input, &input_size, &ch, cur_atm_ptr, 1)) {
        case OP_CANCELLED: return OP_CANCELLED;
        case OP_FAILED: return OP_FAILED;
    }

    printf("%-*s", UI_PROMPT_MSG_LEN, " Enter New PIN:");
    if(pin_input(input, &input_size, &ch, NULL, 0) == OP_CANCELLED) return OP_CANCELLED;
    strcpy(temp_atm.pin, input);

    printf("%-*s", UI_PROMPT_MSG_LEN, " Re-enter New PIN:");
    if(pin_input(input, &input_size, &ch, &temp_atm, 0) == OP_CANCELLED) return OP_CANCELLED;
    strcpy(cur_atm_ptr->pin, temp_atm.pin);
    update_atm_file(cur_index, 2, cur_atm_ptr->pin);

    prnt_ui_line(0);
    printf(" Successfully Changed PIN !\n\n Press any key to return...\n");
    _getch();

    return OP_FINISHED;
}

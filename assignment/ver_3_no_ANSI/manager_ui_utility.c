#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#include "metadata_manager.c"

// Operation states

#define OP_LOOPING      0
#define OP_FINISHED     1
#define OP_FAILED       2 
#define OP_CANCELLED    -1

// UI 

#define UI_WIDTH 50
#define UI_PROMPT_MSG_LEN 23
#define UI_CURRENCY " VND"

// ASCII for keys

#define ESC_KEY         27
#define CTRL_BACKSPACE  23

// Function prototypes

int unbuffered_input(char *target_buffer, int max_size, int input_mode, int is_censored, char first_ch);
int choice_input(int min, int max);
int yes_no_input();

void prnt_line(int size, int double_line);
void prnt_ui_line(int double_line);
void prnt_header();
void prnt_invalid(char *msg, int input_size, char *ch);

void standardize_str(char *str);
void str_to_money(char *target_str, long long int money);
void str_to_line(char *target, int double_line);

// Function definitions
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
            case CTRL_BACKSPACE:
                memset(input, '\0', max_size + 1);
                while(input_size > 0) {
                    printf("\b \b");
                    input_size--;
                }
                break;

            case '\b':
                if(input_size > 0) {
                    printf("\b \b");
                    input[--input_size] = '\0';
                }
                break;

            case ESC_KEY:
                while(input_size > 0) {
                    printf("\b \b");
                    input_size--;
                }
                return OP_CANCELLED;
                break;

            default:
                switch(input_mode) {
                    case 1: if(!isdigit(ch)) continue; break;
                    case 2: if(!isalpha(ch) && !(ch == ' ' && input_size > 0)) continue; break;
                    case 3: if(!isalnum(ch) && !(ch == ' ' && input_size > 0)) continue; break;
                    default: continue; break;
                }
                if(input_size < max_size) {
                    input[input_size++] = ch;
                    input[input_size] = '\0';
                    putchar(is_censored ? '*' : ch);
                }
                break;
        }
    }
    
    strcpy(target_buffer, input);
    return input_size;
}

/**
 * @brief   Keep taking input until condition met or ESC entered. Print "Your choice: " automatically
 * 
 * @return  Min to max, OP_CANCELLED if ESC
 */
int choice_input(int min, int max) {
    char input[2] = "\0", ch = 0;
    int choice, input_size;

    printf(" Your choice: ");
    while (1) {
        input_size = unbuffered_input(input, 1, 1, 0, ch);
        if(input_size == -1) return OP_CANCELLED;

        choice = input_size > 0 ? strtol(input, NULL, 10) : min - 1;
        if(choice >= min && choice <= max) break;

        prnt_invalid("Invalid choice", input_size, &ch);
    }
    putchar('\n');

    return choice;
}

/**
 * @brief   Keep taking input until 'Y', 'y', 'N', 'n', ENTER, ESC entered
 * 
 * @return  1: Yes & ENTER, 0: No & ESC
 */
int yes_no_input() {
    int input_size;
    char input[2];
    while(1) {
        input[0] = '\0';
        input_size = unbuffered_input(input, 1, 2, 0, 0);
        if(input[0] == 'Y' || input[0] == 'y' || input_size == 0) {
            putchar('\n');
            return 1;
        }
        if(input[0] == 'N' || input[0] == 'n' || input_size == OP_CANCELLED) {
            putchar('\n');
            return 0;
        }
        printf("\b %s", input_size > 0 ? "\b" : "");
    }
}

void prnt_line(int size, int double_line) {
    char ch = double_line ? '=' : '-';
    char line[size + 1];
    memset(line, ch, size);
    line[size] = '\0';
    printf("%s\n", line);
}

void prnt_ui_line(int double_line) {
    prnt_line(UI_WIDTH, double_line);
}

/**
 * @brief   Print 'bank name' between 2 lines of '='
 */
void prnt_header() {
    prnt_ui_line(1);
    printf("%*s\n", (UI_WIDTH + strlen(main_meta.bank_name)) / 2, main_meta.bank_name);
    prnt_ui_line(1);
}

/**
 * @brief   Delete current input with 'input_size', print 'msg', wait for user input then delete 'msg' and save said input in 'ch'
 */
void prnt_invalid(char *msg, int input_size, char *ch) {
    while(input_size > 0) {
        printf("\b \b");
        input_size--;
    }
    printf("%s", msg);

    *ch = _getch();
    input_size = strlen(msg);
    while(input_size > 0) {
        printf("\b \b");
        input_size--;
    }
}

/**
 * @brief   Remove following spaces and change all characters to upper
 */
void standardize_str(char *str) {
    char *ptr = str;
    int is_word = 0;

    for(int i = 0; str[i]; i++) {
        if(isalpha(str[i])) {
            *ptr++ = toupper(str[i]);
            is_word = 1;
        }
        else if (isspace(str[i])) {
            if(is_word) *ptr++ = ' ';
            is_word = 0;
        }
    }

    ptr -= !is_word;
    *ptr = '\0';
}

/**
 * @brief   Add dot every 3 digits and "VND" at the end
 * 
 * @param target_str  Should point to sizable buffer
 */
void str_to_money(char *target_str, long long int money) {
    char money_str[main_meta.data_sizes[3] + 15];
    sprintf(money_str, "%lld", money);
    int size = strlen(money_str);

    int dot_count = 0;
    for(int i = 0; i < size; i++) {
        if((size - i) % 3 == 0 && i > 0) {
            memmove(money_str + i + dot_count + 1, money_str + i + dot_count, size - i);
            money_str[i + dot_count++] = '.';
        }
    }
    money_str[size + dot_count] = '\0';
    strcat(money_str, UI_CURRENCY);
    strcpy(target_str, money_str);
}

void str_to_line(char *target, int double_line) {
    int ch = double_line ? '=' : '-';
    memset(target, ch, UI_WIDTH);
    target[UI_WIDTH] = '\n';
    target[UI_WIDTH + 1] = '\0';
}

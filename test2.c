#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define UI_PROMPT_MSG_LENGTH 20
#define UI_NOTI_MSG_LENGTH 50

void atm_input(void *target, size_t length, int digit_only, int return_lld, int exact_length, long long int min_value, char *ui_prompt_msg);

int main() {
    long long int test_lli;
    char test_char[7];

    atm_input(test_char, 15, 0, 0, 0, 0, "Input Name:");
    printf("%s\n", test_char);

    return 0;
}

void atm_input(void *target, size_t length, int digit_only, int return_lld, int exact_length, long long int min_value, char *ui_prompt_msg) {
    size_t input_len = 1;
    size_t buffer_size = 10;
    char ch, *input = malloc(buffer_size * sizeof(char));
    input[0] = '\0';

    printf(" %-*s", UI_PROMPT_MSG_LENGTH, ui_prompt_msg);
    ch = _getch();

    while(ch != 27) {
        if(digit_only ? isdigit(ch) : (isalpha(ch) || ch == ' ')) {
            if(input_len >= buffer_size) {
                buffer_size *= 2;
                input = realloc(input, buffer_size * sizeof(char));
            }
            input[input_len - 1] = ch;
            input[input_len++] = '\0';

            putchar(ch);
        }
        else if(ch == '\n' || ch == '\r') {
            int valid = exact_length ? input_len == length + 1 : input_len <= length + 1;
            if(return_lld) valid = strtoll(input, NULL, 10) >= min_value;

            if(valid) break;

            printf(
                "\r %*s\r %-*s%s",
                UI_PROMPT_MSG_LENGTH + input_len, "",
                UI_PROMPT_MSG_LENGTH, ui_prompt_msg,
                "Invalid, please re-enter"
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

        ch = _getch();
    }

    if(return_lld) *(long long int *)target = strtoll(input, NULL, 10);
    else strcpy((char *)target, input);

    putchar('\n');
    free(input);
}

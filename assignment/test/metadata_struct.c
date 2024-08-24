#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <math.h>

const char *BANK_NAME = "VTC Academy Bank";
const char *METADATA_FILE = "test_metadata.dat";
const char *ATM_ACCOUNT_FILE = "test_atm_accounts.dat";

typedef struct ATM
{
    char *name;
    char *account;
    char *pin;
    long long int balance;
} ATM;

typedef struct ATM_METAMETA
{
    size_t meta_len;
    size_t name_len;
    size_t account_len;
    size_t pin_len;
    size_t balance_len;

    char *name_tag;
    char *account_tag;
    char *pin_tag;
    char *balance_tag;

    size_t name_tag_len;
    size_t account_tag_len;
    size_t pin_tag_len;
    size_t balance_tag_len;

    size_t total_len;
} ATM_METAMETA;

size_t ATM_META_LEN = 2;
size_t ATM_NAME_LEN = 30;
size_t ATM_ACCOUNT_LEN = 14;
size_t ATM_PIN_LEN = 6;
size_t ATM_BALANCE_LEN = 18;
#define ATM_TOTAL_META_LEN ATM_NAME_LEN + ATM_ACCOUNT_LEN + ATM_PIN_LEN + ATM_BALANCE_LEN

char *ATM_NAME_TAG = "name:";
char *ATM_ACCOUNT_TAG = "ACCOUNT:";
char *ATM_PIN_TAG = "PIN:";
char *ATM_BALANCE_TAG = "bal:";

ATM_METAMETA meta1;
ATM_METAMETA meta2;

void append_atm(ATM *atm);
void read_atm(ATM *atm);

void meta1_init();
void meta1_fprnt();
int meta2_fscn();
void meta2_free();

int main() {
    //goto test_label;

    ATM test_append = {
        .name = "test account",
        .account = "12341234123412",
        .pin = "000000",
        .balance = 10000000
    };
    ATM test_read;

    meta1_init();

    meta1_fprnt();

    meta2_fscn();

    read_atm(&test_read);

    append_atm(&test_read);

    meta2_free();

    test_label:
    goto done_testing_label;

    printf("%%*[^\\n]");

    done_testing_label:
    return 0;
}

void append_atm(ATM *atm) {
    FILE *file = fopen(ATM_ACCOUNT_FILE, "a+");

    fprintf(file,
        "%*s%*s %*s%*s %*s%*s %*s%*lld\n",
        meta2.name_tag_len, meta2.name_tag, meta2.name_len, atm->name,
        meta2.account_tag_len, meta2.account_tag, meta2.account_len, atm->account,
        meta2.pin_tag_len, meta2.pin_tag, meta2.pin_len, atm->pin,
        meta2.balance_tag_len, meta2.balance_tag, meta2.balance_len, atm->balance
    );

    fclose(file);
}

void read_atm(ATM *atm) {
    char format_buffer[100], line[meta2.total_len + 5];

    FILE *file = fopen(ATM_ACCOUNT_FILE, "r");
    if(file == NULL) return;

    int read = 0;
    while(!read) {
        if(fscanf(file, "%[^\n]\n", line) == 1) {
            atm->name = malloc(meta2.name_len + 1);
            atm->account = malloc(meta2.account_len + 1);
            atm->pin = malloc(meta2.pin_len + 1);
            char balance_buffer[meta2.balance_len + 1];

            sprintf(format_buffer, 
                "%s%%%d[^\n] %s%%%ds %s%%%ds %s%%%d[^\n]%%*[^\n]\n",
                meta2.name_tag, meta2.name_len,
                meta2.account_tag, meta2.account_len,
                meta2.pin_tag, meta2.pin_len,
                meta2.balance_tag, meta2.balance_len
            );

            sscanf(line, format_buffer, atm->name, atm->account, atm->pin, balance_buffer);
            atm->balance = strtoll(balance_buffer, NULL, 10);
            read = 1;
        }

    }

}

int meta2_fscn() {
    FILE *file = fopen(METADATA_FILE, "r+");

    if(file == NULL) return 1;

    char ch = fgetc(file);
    if(ch == EOF) return 1;
    
    fseek(file, 0, 0);

    char format_buffer[100];

    fscanf(file, "%1d", &meta2.meta_len);

    sprintf(format_buffer,
        "%%%dd%%%dd%%%dd%%%dd%%%dd%%%dd%%%dd%%%dd\\n",
        meta2.meta_len, meta2.meta_len, meta2.meta_len, meta2.meta_len,
        meta2.meta_len, meta2.meta_len, meta2.meta_len, meta2.meta_len
    );
    if(fscanf(file, format_buffer,
        &meta2.name_len, &meta2.account_len, &meta2.pin_len, &meta2.balance_len,
        &meta2.name_tag_len, &meta2.account_tag_len, &meta2.pin_tag_len, &meta2.balance_tag_len
    ) != 8) return 1;
    meta2.total_len = meta2.name_len + meta2.account_len + meta2.pin_len + meta2.balance_len;
    meta2.total_len += meta2.name_tag_len + meta2.account_tag_len + meta2.pin_tag_len + meta2.balance_tag_len;

    sprintf(format_buffer,
        "%%%ds%%%ds%%%ds%%%ds\\n",
        meta2.name_tag_len, meta2.account_tag_len, meta2.pin_tag_len, meta2.balance_tag_len
    );

    meta2.name_tag = malloc(meta2.name_tag_len + 1);
    meta2.account_tag = malloc(meta2.account_tag_len + 1);
    meta2.pin_tag = malloc(meta2.pin_tag_len + 1);
    meta2.balance_tag = malloc(meta2.balance_tag_len + 1);

    if(fscanf(file, format_buffer,
        meta2.name_tag, meta2.account_tag, meta2.pin_tag, meta2.balance_tag
    ) != 4) return 1;

    fclose(file);
    return 0;
}

void meta2_free() {
    free(meta2.name_tag);
    free(meta2.account_tag);
    free(meta2.pin_tag);
    free(meta2.balance_tag);
}

void meta1_init() {
    meta1.meta_len = ATM_META_LEN,
    meta1.name_len = ATM_NAME_LEN,
    meta1.account_len = ATM_ACCOUNT_LEN,
    meta1.pin_len = ATM_PIN_LEN,
    meta1.balance_len = ATM_BALANCE_LEN,

    meta1.name_tag = ATM_NAME_TAG,
    meta1.account_tag = ATM_ACCOUNT_TAG,
    meta1.pin_tag = ATM_PIN_TAG,
    meta1.balance_tag = ATM_BALANCE_TAG,

    meta1.name_tag_len = strlen(meta1.name_tag);
    meta1.account_tag_len = strlen(meta1.account_tag);
    meta1.pin_tag_len = strlen(meta1.pin_tag);
    meta1.balance_tag_len = strlen(meta1.balance_tag);
}

void meta1_fprnt() {
    FILE *file = fopen(METADATA_FILE, "w+");

    fprintf(file,
        "%d%0*d%0*d%0*d%0*d%0*d%0*d%0*d%0*d\n"
        "%s%s%s%s\n",

        meta1.meta_len,
        meta1.meta_len, meta1.name_len,
        meta1.meta_len, meta1.account_len,
        meta1.meta_len, meta1.pin_len,
        meta1.meta_len, meta1.balance_len,
        meta1.meta_len, meta1.name_tag_len,
        meta1.meta_len, meta1.account_tag_len,
        meta1.meta_len, meta1.pin_tag_len,
        meta1.meta_len, meta1.balance_tag_len,

        meta1.name_tag,
        meta1.account_tag,
        meta1.pin_tag,
        meta1.balance_tag
    );

    fclose(file);
}

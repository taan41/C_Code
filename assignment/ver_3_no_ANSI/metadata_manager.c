#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Defines

#ifdef _WIN32
    #include <direct.h>  // For _mkdir on Windows
    #define MKDIR(dir) _mkdir(dir)
#else
    #include <sys/stat.h>  // For mkdir on Unix/Linux
    #include <sys/types.h>
    #define MKDIR(dir) mkdir(dir, 0777)
#endif

#define META_FILE "data\\metadata.dat"

/**
 * @brief   0: Name, 1: Account, 2: PIN, 3: Balance, 4: PIN Attempts
 */
typedef struct ATM_METADATA{
    char separator;
    size_t data_sizes[5];
    char *tags[5];
    long long int bal_create_min;
    long long int bal_withdraw_max;
    char *bank_name;
} ATM_METADATA;

// Function prototypes

void init_meta_manager();

void meta_to_file(ATM_METADATA *meta, char *file_name);
int meta_from_file(ATM_METADATA *meta, char *file_name);
int meta_cmp(ATM_METADATA *meta1, ATM_METADATA *meta2);

// Global variables

ATM_METADATA main_meta, save_meta;
ATM_METADATA default_meta = {
    .separator = ':',
    .data_sizes = {30, 14, 6, 18, 2},
    .tags = {"name", "account", "pin", "balance", "attempt"},
    .bal_create_min = 50000,
    .bal_withdraw_max = 25000000,
    .bank_name = "VTC Academy Bank"
};

// Function definitions

void init_meta_manager() {
    MKDIR("data");
    switch(meta_from_file(&main_meta, META_FILE)) {
        case 0: break;
        case 1: case 2:
            main_meta = default_meta;
            meta_to_file(&main_meta, META_FILE);
            break;
    }
}

/**
 * @return 0: No error, 1: Error opening file, 2: Error reading file
 */
int meta_from_file(ATM_METADATA *meta, char *file_name) {
    FILE *file = fopen(file_name, "r");

    if(file == NULL) return 1;

    meta->separator = fgetc(file);
    if(meta->separator == '\n' || meta->separator == EOF) return 2;
    fgetc(file);

    char format[9], buffer[100];
    sprintf(format, "%%[^|]|");
    for(int i = 0; i < 5; i++) {
        if(fscanf(file, format, buffer) != 1) return 2;
        meta->data_sizes[i] = strtol(buffer, NULL, 10);
    }

    for(int i = 0; i < 5; i++) {
        if(fscanf(file, format, buffer) != 1) return 2;
        meta->tags[i] = strdup(buffer);
    }
    
    if(fscanf(file, format, buffer) != 1) return 2;
    meta->bal_create_min = strtoll(buffer, NULL, 10);

    if(fscanf(file, format, buffer) != 1) return 2;
    meta->bal_withdraw_max = strtoll(buffer, NULL, 10);

    if(fscanf(file, format, buffer) != 1) return 2;
    meta->bank_name = strdup(buffer);

    fclose(file);
    return 0;
}

void meta_to_file(ATM_METADATA *meta, char *file_name) {
    FILE *file = fopen(file_name, "w+");

    char s = meta->separator;
    fputc(s, file);
    fputc('|', file);

    for(int i = 0; i < 5; i++) fprintf(file, "%d|", meta->data_sizes[i], s);
    for(int i = 0; i < 5; i++) fprintf(file, "%s|", meta->tags[i], s);
    fprintf(file, "%lld|", meta->bal_create_min, s);
    fprintf(file, "%lld|", meta->bal_withdraw_max, s);
    fprintf(file, "%s|", meta->bank_name, s);

    if(strcmp(file_name, META_FILE) == 0) fprintf(file,
        "\n\n"
        "## Editable metadata\n"
        "## '|' seperates each metadata\n"
        "## List of metadata:   '(char)seperator'\n"
        "##                     '(int)data_sizes[5]' '(char *)tags[5]'\n"
        "##                     '(long long int)bal_create_min' '(long long int)bal_withdraw_max'\n"
        "##                     '(char *)bank_name'\n"
        "## Each element of data_sizes[] and tags[] represents: 0 = name, 1 = account no, 2 = pin code, 3 = balance, 4 = attempt\n",
        meta->separator
    );
    else fprintf(file, "    ## Read-only metadata\n");
    fclose(file);
}

/**
 * @return  1: Same, 0: Diff
 */
int meta_cmp(ATM_METADATA *meta1, ATM_METADATA *meta2) {
    for(int i = 0; i < 5; i++) {
        if(meta1->data_sizes[i] != meta2->data_sizes[i]) return 0;
        if(strcmp(meta1->tags[i], meta2->tags[i]) != 0) return 0;
    }

    int cmp = meta1->separator == meta2->separator;
    if(cmp) cmp = meta1->bal_create_min == meta2->bal_create_min;
    if(cmp) cmp = meta1->bal_withdraw_max == meta2->bal_withdraw_max;
    if(strcmp(meta1->bank_name, meta2->bank_name) != 0) return 0;

    return cmp;
}

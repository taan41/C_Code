#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define META_FILE "metadata.dat"

/**
 * @brief   0: Name, 1: Account, 2: PIN, 3: Balance
 */
typedef struct ATM_METADATA{
    char separator;
    size_t data_sizes[4];
    char *tags[4];
    long long int bal_min;
    char *bank_name;
} ATM_METADATA;

ATM_METADATA cur_meta, old_meta;
ATM_METADATA default_meta = {
    .separator = ':',
    .data_sizes = {30, 14, 6, 18},
    .tags = {"name", "account", "pin", "balance"},
    .bal_min = 50000,
    .bank_name = "VTC Academy Bank"
};

void init_meta_manager();

void meta_to_file(ATM_METADATA *meta, char *file_name);
int meta_from_file(ATM_METADATA *meta, char *file_name);
int meta_cmp(ATM_METADATA *meta1, ATM_METADATA *meta2);

void init_meta_manager() {
    if(meta_from_file(&cur_meta, META_FILE) == 1) {
        cur_meta = default_meta;
        meta_to_file(&cur_meta, META_FILE);
    }
}

int meta_from_file(ATM_METADATA *meta, char *file_name) {
    FILE *file = fopen(file_name, "r");

    if(file == NULL) {
        printf("Error opening %s\n", file_name);
        return 1;
    }

    meta->separator = fgetc(file);
    if(meta->separator == '\n' || meta->separator == EOF) {
        printf("Error reading %s\n", file_name);
        return 1;
    }

    char format[9], buffer[100];
    sprintf(format, "%%[^'%c']%c", meta->separator, meta->separator);
    for(int i = 0; i < 4; i++) {
        fscanf(file, format, buffer);
        meta->data_sizes[i] = strtol(buffer, NULL, 10);
    }

    for(int i = 0; i < 4; i++) {
        fscanf(file, format, buffer);
        meta->tags[i] = strdup(buffer);
    }
    
    fscanf(file, format, buffer);
    meta->bal_min = strtoll(buffer, NULL, 10);

    fscanf(file, format, buffer);
    meta->bank_name = strdup(buffer);

    fclose(file);
    return 0;
}

void meta_to_file(ATM_METADATA *meta, char *file_name) {
    FILE *file = fopen(file_name, "w+");

    char s = meta->separator;
    fputc(s, file);

    for(int i = 0; i < 4; i++) fprintf(file, "%d%c", meta->data_sizes[i], s);
    for(int i = 0; i < 4; i++) fprintf(file, "%s%c", meta->tags[i], s);
    fprintf(file, "%lld%c", meta->bal_min, s);
    fprintf(file, "%s%c", meta->bank_name, s);

    fputc('\n', file);
    fclose(file);
}

/**
 * @return  1: Same, 0: Diff
 */
int meta_cmp(ATM_METADATA *meta1, ATM_METADATA *meta2) {
    for(int i = 0; i < 4; i++) {
        if(meta1->data_sizes[i] != meta2->data_sizes[i]) return 0;
        if(strcmp(meta1->tags[i], meta2->tags[i]) != 0) return 0;
    }
    if(strcmp(meta1->bank_name, meta2->bank_name) != 0) return 0;
    return meta1->separator == meta2->separator && meta1->bal_min == meta2->bal_min;
}

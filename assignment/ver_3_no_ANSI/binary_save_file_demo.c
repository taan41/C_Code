#include "data_manager.c"

#define FILE_NAME "data\\account-number.bin"

void write_custom_data() {
    FILE *file = fopen(FILE_NAME, "wb");

    size_t custom_list_size = 4;

    ATM *custom_list = (ATM *)malloc(custom_list_size * sizeof(ATM));

    custom_list[0].name = "LE DUONG ANH";
    custom_list[0].account = "51421803041119";
    custom_list[0].pin = "731512";
    custom_list[0].balance = 923700000;
    custom_list[0].PIN_attempt = 7;

    custom_list[1].name = "HOANG DUONG HA";
    custom_list[1].account = "99649722059935";
    custom_list[1].pin = "391860";
    custom_list[1].balance = 504600000;
    custom_list[1].PIN_attempt = 7;

    custom_list[2].name = "DANG DUONG HOA";
    custom_list[2].account = "54886320803532";
    custom_list[2].pin = "324631";
    custom_list[2].balance = 922800000;
    custom_list[2].PIN_attempt = 7;

    custom_list[3].name = "TRAN VAN HA";
    custom_list[3].account = "75886947055919";
    custom_list[3].pin = "818018";
    custom_list[3].balance = 30600000;
    custom_list[3].PIN_attempt = 7;
    
    fwrite(&custom_list_size, sizeof(size_t), 1, file);
    fwrite(custom_list, sizeof(ATM), custom_list_size, file);

    fclose(file);
    free(custom_list);
}

void write_data_from_data_manager() {
    init_data_manager();

    FILE *file = fopen(FILE_NAME, "wb");

    fwrite(&atm_list_size, sizeof(size_t), 1, file);
    fwrite(atm_list, sizeof(ATM), atm_list_size, file);

    fclose(file);
}

int main() {
    MKDIR("data");

    // write_custom_data();
    write_data_from_data_manager();

    FILE *file = fopen(FILE_NAME, "rb");

    size_t test_list_size;
    fread(&test_list_size, sizeof(size_t), 1, file);

    ATM *test_list = (ATM *)malloc(test_list_size * sizeof(ATM));
    printf("Elements read: %d\n", fread(test_list, sizeof(ATM), test_list_size, file));

    for(int i = 0; i < test_list_size; i++) {
        printf("Name: %-30s Account: %-14s PIN: %-6s Balance: %-18lld Attempts: %d\n",
               test_list[i].name,
               test_list[i].account,
               test_list[i].pin,
               test_list[i].balance,
               test_list[i].PIN_attempt);
    }

    fclose(file);
    free(test_list);
}
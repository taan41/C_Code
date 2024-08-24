#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NUM 14
#define MAX_PIN 6
int isValidAccountNumber(char *accountNumber){
    return strlen(accountNumber) == 14 && strspn(accountNumber,"0123456789") ==14;
}
int isValidPinCode(char *pinCode){
    return strlen(pinCode) == 6 && strspn(pinCode,"0123456789") == 6;
}
void deleInput(int ch)
{
    while((ch = getchar()) != '\n' && ch != EOF);
}

int isValidAccountName(char *accountName)
{
    for (int i = 0; i < strlen(accountName); i++) {
        if (!isalpha(accountName[i]) && !isspace(accountName[i])) {
            return 0;
        }
    }
    return 1;
}

void saveToFile(char *accountName, char *accountNumber, char *pinCode, long int accBalance)
{
    FILE *file = fopen("account-number.dat", "a+");
    if (file == NULL)
    {
        printf("Cannot open file!\n");
        return;
    }
    fprintf(file, "Ten tai khoan: %s\n", accountName);
    fprintf(file, "So tai khoan: %s\n", accountNumber);
    fprintf(file, "Ma pin: %s\n", pinCode);
    fprintf(file, "So du: %ld\n", accBalance);
    fclose(file);
    printf("Đa luu thong tin tai khoan vao file account-number.dat\n");
}


int main()
{
    char accountName[50];
    char accountNumber[MAX_NUM+2];
    char pinCode[MAX_PIN+2];
    long int accBalance;
    char saveChoice;

    printf("==================================\n");
    printf("\tVTC Academy Bank\n");
    printf("==================================\n");
    printf("\tCreate ATM Cards\n");
    printf("----------------------------------\n");
    printf("Input Account Name: ");
    do {
        fgets(accountName, sizeof(accountName), stdin);
        accountName[strcspn(accountName, "\n")] = '\0';
        if (!isValidAccountName(accountName)) {
            printf("Invalid account name, re-enter: ");
        }
    } while (!isValidAccountName(accountName));
    printf("Input Account No(14 digital): ");
do
{
    fgets(accountNumber, sizeof(accountNumber), stdin);
    if (strlen(accountNumber) > MAX_NUM && accountNumber[MAX_NUM] != '\n')
    {
        deleInput(0);
        printf("Invalid account number, re-enter: ");
        continue;
    } 
    accountNumber[strcspn(accountNumber, "\n")] = '\0';
    if (!isValidAccountNumber(accountNumber))
    {
        printf("Invalid account number, re-enter: ");
    }
} while (!isValidAccountNumber(accountNumber));

printf("Input Code(6 number): ");
do
{
    fgets(pinCode, sizeof(pinCode), stdin);
    if (strlen(pinCode) > MAX_PIN && pinCode[MAX_PIN] != '\n')
    {
        deleInput(0);
        printf("Invalid Pin code, re-enter:");
        continue;
    }
    pinCode[strcspn(pinCode, "\n")] = '\0';
    if (!isValidPinCode(pinCode))
    {
        printf("Invalid Pin code, re-enter:");
    }
} while (!isValidPinCode(pinCode));

do
{
    printf("Input balance (>50000): ");
    scanf("%ld", &accBalance);
} while (accBalance < 50000);

printf("----------------------------------\n");
printf("Create ATM successfully\n");
printf("Do you want to create another ATM account (Y/N)? ");
deleInput(0);
saveChoice = getchar();
if ( saveChoice == 'Y' || saveChoice == 'y')
  {
    saveToFile(accountName, accountNumber, pinCode, accBalance);
  }else
  {
    printf("Goodbye\n");
  }
  return 0;

}
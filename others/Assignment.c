#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_LENGTH 256
#define MAX_USERS 50              // Maximum amount of user accounts that can be stored in the program
#define MAX_TRANSACTIONS 1000      // Maximum amount of transactions that can be stored in the program  
#define ACCOUNT_NUMBER_LENGTH 14  // Length for account number
#define PIN_LENGTH 6              // Length for PIN
#define TRANSACTION_ID_LENGTH 10  // Length for transaction id
#define MIN_BALANCE 50000         // Minimum amount of input Balance
#define COST 1000                 // Cost for using the ATM features (Withdrawal and Transfer)
#define VAT 100                   // VAT for cost    
#define FORMAT 40                 // Length of = for formatting

struct userAccount
{
    char userName[50];
    char accountNumber[ACCOUNT_NUMBER_LENGTH + 2]; // Account for the NULL zero and the \n when input
    char accountPin[PIN_LENGTH + 2]; // Account for the NULL zero and the \n when input
    long long accountBalance;
};

struct userTransaction
{
    char userName[50];
    char dateBuffer[80];
    char timeBuffer[80];
    char censoredAccount[ACCOUNT_NUMBER_LENGTH + 1]; // Account for the NULL zero
    char transactionID[TRANSACTION_ID_LENGTH + 1]; //10 digits transaction id and account for the NULL zero
    long long amount; 
};

/* Function Prototypes*/
void outMenuUI();         // Display outermost Menu with login and create account
void accCreateUI();       // Display UI create account
void accCreate(struct userAccount users[], int *userCount);         // Create account function
void accDispUI(struct userAccount account);         // Display account info UI
int accDispYN();          // Yes or no for printing confirmation
void fileSave(struct userAccount users[], int userCount);          // Save account information to file function 
void fileLoad(struct userAccount users[], int *userCount);         // Load data file for reading
void logInUI();           // Display login UI (login, forgot password, EXIT) 
void accAuthUI();         // Display account authentication UI (input ID and password, EXIT)
int accAuthIn(struct userAccount users[], int userCount, struct userAccount *loggedInAccount);    // Authentication input function   
void forgotPin(struct userAccount users[], int userCount); 
void inMenuUI(struct userAccount account);          // Display inner menu with the main ATM features (Display balance, withdrawal, transfer, change pin, EXIT) 
long long getNextTransactionID();   // To generate unique transaction id between program runs
void saveTransaction(struct userTransaction *transaction, const char *userName, const char *accountNumber, long long amount);     // To save transaction to file and printing receipt
void balDispUI(struct userAccount account);         // Balance display UI
void balWithUI(struct userAccount *account, struct userAccount users[], int userCount, struct userTransaction * transaction);        // Balance withdrawal UI (6 premade options and a customizable option) 
void balTransUI(struct userAccount *account, struct userAccount users[], int userCount, struct userTransaction * transaction);        // Balance Transfer UI (disp account number, account balance, input destination account number, input tranfer amount)
void chgPinUI(struct userAccount *account, struct userAccount users[], int userCount);         // Forgot pin input (input account number, new pin, confirm new pin, change succesfully, EXIT)
void confirmExit();            // Yes or no confirmation to exit the program
void clearInputBuffer();  // To clear input buffet along the program    


int main()
{   
    FILE * file = fopen("account-number.dat", "r+");
    if (file == NULL) 
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    FILE * file1 = fopen("transaction-record.dat", "a+");
    if (file1 == NULL) 
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    FILE * file2 = fopen("transactionid.dat", "r");
    if (file2 == NULL) 
    {
        printf("Error: Could not open file.\n");
        return 1;
    }    

    /* Declares an array named users that can hold up to MAX_USERS number of userAccount structures.*/
    struct userAccount users[MAX_USERS];
    int userCount = 0; /* keeps track of the number of user accounts currently stored in the users array. */

    /* Store the details of the currently logged-in user. */
    struct userAccount loggedInAccount; 
    int loggedIn = 0; /* Indicate whether a user is currently logged in or not. */

    /* Declares an array named users that can hold up to MAX_USERS number of userAccount structures.*/
    struct userTransaction transaction[MAX_TRANSACTIONS];

    // Load user data from file
    fileLoad(users, &userCount);

    // Outer Menu
    while (1)  
    {
        outMenuUI(); // 1. Create acc  2. Log in   3. Exit
        int outMenuChoice;

        printf("Please choose an option: ");
        if (scanf("%d", &outMenuChoice) != 1) 
        {
            printf("Invalid input. Please enter a number: \n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (outMenuChoice)
        {
            case (1): // Create Account
                accCreateUI();
                accCreate(users, &userCount);
                accDispUI(users[userCount-1]);

                if (accDispYN())
                {
                    printf("You selected Yes. Account created succesfully!\n");
                    fileSave(users, userCount);
                    printf("Press any key to go back to the Login menu...\n");
                    getchar();  // Waits for any key press    
                }
                    else
                    {
                        printf("You selected No. Back to Account Creation");
                    }
                break;

            case (2): // Login
                logInUI(); // 1. Login  2. Forgot Password   3. Exit (back to outer Menu)
                while (1) 
                {
                    int logInChoice;
                    printf("Please choose an option: ");
                    if (scanf("%d", &logInChoice) != 1) 
                    {
                        printf("Invalid input. Please enter a number: \n");
                        clearInputBuffer();
                        continue;
                    }
                    clearInputBuffer();

                    switch (logInChoice)
                    {
                        case (1):  // Login
                            loggedIn = accAuthIn(users, userCount, &loggedInAccount);

                            if (loggedIn)
                            {
                                while (1)
                                {
                                    inMenuUI(loggedInAccount); // 1. Display Balance  2. Withdrawal  3. Transfer   4. Change pin  5. Exit
                                    int InMenuChoice;

                                    printf("Please choose an option: ");
                                    if (scanf("%d", &InMenuChoice) != 1) 
                                    {
                                        printf("Invalid input. Please enter a number: \n");
                                        clearInputBuffer();
                                        continue;
                                    }
                                    clearInputBuffer();

                                    switch (InMenuChoice)
                                    {
                                        case (1): // Display Balance
                                            balDispUI(loggedInAccount);
                                            printf("Press any key to back to ATM menu...\n");
                                            getchar();  // Waits for any key press
                                        break;

                                        case (2): // Withdrawal                                
                                            balWithUI(&loggedInAccount, users, userCount, &transaction[0]); // 1. 100k   2. 200k   3. 500k   4. 1000k   5. 2000k   6. Other   7. Back
                                            break;

                                        case (3): // Transfer
                                            balTransUI(&loggedInAccount, users, userCount, &transaction[0]);
                                            break;

                                        case (4):  // Change pin
                                            chgPinUI(&loggedInAccount, users, userCount);
                                            break;

                                        case (5): // Exit the program
                                            confirmExit();

                                        default:
                                            printf("Invalid option. Please try again.\n");
                                            break;
                                    }
                                }
                            }
                            break;    

                        case (2): // Forgot Password
                            forgotPin(users, userCount);
                            break;

                        case (3):  // Exit
                            printf("Returning to the Main Menu... \n");
                            break;

                        default:
                            printf("Invalid option. Please try again.\n");
                            break;
                    }
                    if (logInChoice == 3) break; // Exits the logInUI loop
                }        
                break;

            case (3): // Exit program
                printf("Exiting to Main Menu... \n");
                fclose(file);   // Close the file when finished with the program
                return 0; // Exit to Main Menu

            default:
                printf("Invalid option. Please try again.\n");
                break;    
        }
    }

    return 0;
}   

void outMenuUI()         // Display outermost Menu with login and create account
{   
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("1. Create Account\n");
    printf("2. Log In\n");
    printf("3. Exit\n"); 
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");   
}

void accCreateUI()       // Display UI create account
{
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Create Account\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
}

void accCreate(struct userAccount users[], int *userCount) 
{
    struct userAccount newUser;
    int validInput = 0;
    int accountExists = 0;

    // Read existing accounts from the file
    FILE *file = fopen("account-number.dat", "r");
    if (file == NULL) 
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    // Load existing users from the file
    while (fscanf(file, "%49[^,],%14[^,],%6[^,],%lld\n", newUser.userName, newUser.accountNumber, newUser.accountPin, &newUser.accountBalance) == 4) 
    {
        users[*userCount] = newUser;
        (*userCount)++;
    }
    fclose(file);

    // Input and validate account details
    while (1) {
        // Input account name
        printf("Enter Account Name: ");
        if (fgets(newUser.userName, sizeof(newUser.userName), stdin) != NULL) {
            newUser.userName[strcspn(newUser.userName, "\n")] = '\0';
        } else {
            printf("Error reading input.\n");
            return;
        }

        // Check if the account name already exists
        int nameExists = 0;
        for (int i = 0; i < *userCount; i++) {
            if (strcmp(users[i].userName, newUser.userName) == 0) {
                nameExists = 1;
                break;
            }
        }
        if (nameExists) {
            printf("Account name already exists. Please choose a different name.\n");
            continue;
        }

        // Input and validate account number
        validInput = 0;
        while (!validInput) {
            printf("Enter Account Number (%d digits): ", ACCOUNT_NUMBER_LENGTH);

            if (fgets(newUser.accountNumber, sizeof(newUser.accountNumber), stdin) != NULL) {
                size_t len = strcspn(newUser.accountNumber, "\n");
                if (newUser.accountNumber[len] == '\n') {
                    newUser.accountNumber[len] = '\0';
                } else {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
                    printf("Invalid account number. Please enter exactly %d digits.\n", ACCOUNT_NUMBER_LENGTH);
                    continue;
                }

                if (strlen(newUser.accountNumber) == ACCOUNT_NUMBER_LENGTH &&
                    strspn(newUser.accountNumber, "0123456789") == ACCOUNT_NUMBER_LENGTH) {
                    accountExists = 0;
                    for (int i = 0; i < *userCount; i++) {
                        if (strcmp(users[i].accountNumber, newUser.accountNumber) == 0) {
                            accountExists = 1;
                            break;
                        }
                    }

                    if (accountExists) {
                        printf("Account number already exists. Please choose a different number.\n");
                    } else {
                        validInput = 1;  // Valid account number
                    }
                } else {
                    printf("Invalid account number. Please enter exactly %d digits.\n", ACCOUNT_NUMBER_LENGTH);
                }
            } else {
                printf("Error reading input. Please try again.\n");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) { /* discard */ }
            }
        }

        // Input and validate PIN
        validInput = 0;
        char confirmPin[PIN_LENGTH + 1];

        while (!validInput) {
            printf("Enter PIN (%d digits): ", PIN_LENGTH);
            if (fgets(newUser.accountPin, sizeof(newUser.accountPin), stdin) != NULL) {
                if (newUser.accountPin[strcspn(newUser.accountPin, "\n")] != '\n') {
                    printf("Invalid PIN. Please enter exactly %d digits.\n", PIN_LENGTH);
                    clearInputBuffer();
                    continue;
                }
                newUser.accountPin[strcspn(newUser.accountPin, "\n")] = '\0';
            } else {
                printf("Error reading input. Please try again.\n");
                continue;
            }

            if (strlen(newUser.accountPin) == PIN_LENGTH && strspn(newUser.accountPin, "0123456789") == PIN_LENGTH) {
                printf("Confirm PIN: ");
                if (fgets(confirmPin, sizeof(confirmPin), stdin) == NULL) {
                    printf("Error reading input. Please try again.\n");
                    clearInputBuffer();
                    continue;
                }
                confirmPin[strcspn(confirmPin, "\n")] = '\0';

                if (strlen(confirmPin) == PIN_LENGTH && strspn(confirmPin, "0123456789") == PIN_LENGTH) {
                    if (strcmp(newUser.accountPin, confirmPin) == 0) {
                        validInput = 1;
                    } else {
                        printf("PINs do not match. Please try again.\n");
                    }
                } else {
                    printf("Invalid confirmation PIN. Please enter exactly %d digits.\n", PIN_LENGTH);
                }
            } else {
                printf("Invalid PIN. Please enter exactly %d digits.\n", PIN_LENGTH);
            }
        }

        // Input and validate initial balance
        validInput = 0;
        while (!validInput) {
            printf("Enter initial Balance (VND): ");
            if (scanf("%lld", &newUser.accountBalance) != 1) {
                printf("Invalid input. Please enter a numeric value.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            if (newUser.accountBalance >= MIN_BALANCE) {
                validInput = 1;
            } else {
                printf("Invalid balance. Please enter an amount greater than or equal to %lld.\n", MIN_BALANCE);
            }
        }

        // Add the new user to the array
        users[*userCount] = newUser;
        (*userCount)++;
        break;
    }
}

void accDispUI(struct userAccount account)         // Display account info UI
{   

    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");;
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Create ATM Card\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Account Name:     %s\n", account.userName);
    printf("Account No:       %s\n", account.accountNumber);
    printf("Account PIN Code: %s\n", account.accountPin);
    printf("Account Balance:  %lld\n", account.accountBalance);
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
}

int accDispYN()         // Yes or no for printing confirmation
{
    char response;
    printf("Would you like to save this account? (Y/N): ");
    while (1) 
    {
        response = getchar();
        // Flush the input buffer to handle extra characters
        while (getchar() != '\n'); 

        // Convert the response to lowercase for easy comparison
        response = tolower(response);
        
        if (response == 'y') 
        {
            return 1;  // Return 1 for 'yes'
        } 
        else if (response == 'n') 
            {
            return 0;  // Return 0 for 'no'
            } 
        else 
        {
            printf("Invalid input. Please enter 'y' for yes or 'n' for no: ");
        }
    }
    
}

void fileSave(struct userAccount users[], int userCount)          // Save account information to file function
{
    FILE *file = fopen("account-number.dat", "w");
    if (file == NULL) 
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s,%s,%s,%lld\n", users[i].userName, users[i].accountNumber, users[i].accountPin, users[i].accountBalance);
    }

    fclose(file);
}

void fileLoad(struct userAccount users[], int *userCount) {
    FILE *file = fopen("account-number.dat", "r");
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    *userCount = 0;
    while (fscanf(file, "%49[^,],%14[^,],%6[^,],%lld\n",
                  users[*userCount].userName,
                  users[*userCount].accountNumber,
                  users[*userCount].accountPin,
                  &users[*userCount].accountBalance) == 4) {
        (*userCount)++;
        if (*userCount >= MAX_USERS) {
            printf("Maximum user limit reached.\n");
            break;
        }
    }

    fclose(file);
}

void logInUI()
{
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Login\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("1. Account Authentication\n");
    printf("2. Forgot PIN\n");
    printf("3. Return to Main Menu\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
}

void accAuthUI()         // Display account authentication UI (input ID and password, EXIT)
{
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Account Authentication\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
}

int accAuthIn(struct userAccount users[], int userCount, struct userAccount *loggedInAccount)         // Authentication input function   
{
    char accountNumber[ACCOUNT_NUMBER_LENGTH + 2];
    char pin[PIN_LENGTH + 2];

    printf("Enter Account Number: ");
    if (fgets(accountNumber, sizeof(accountNumber), stdin) != NULL) {
        accountNumber[strcspn(accountNumber, "\n")] = '\0'; // Remove newline character
    } else {
        printf("Error reading input.\n");
        return 0;
    }

    printf("Enter PIN: ");
    if (fgets(pin, sizeof(pin), stdin) != NULL) {
        pin[strcspn(pin, "\n")] = '\0'; // Remove newline character
    } else {
        printf("Error reading input.\n");
        return 0;
    }

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].accountNumber, accountNumber) == 0 &&
            strcmp(users[i].accountPin, pin) == 0) {
            *loggedInAccount = users[i];
            return 1; // Login successful
        }
    }

    printf("Invalid account number or PIN.\n");
    return 0; // Login failed
}

void forgotPin(struct userAccount users[], int userCount)           // Forgot pin UI (input account number, new pin, confirm new pin, change succesfully, EXIT)
{
    char accountNumber[ACCOUNT_NUMBER_LENGTH + 2];
    char newPin[PIN_LENGTH + 2];
    char confirmPin[PIN_LENGTH + 2];

    printf("Enter Account Number: ");
    if (fgets(accountNumber, sizeof(accountNumber), stdin) != NULL) {
        accountNumber[strcspn(accountNumber, "\n")] = '\0'; // Remove newline character
    } else {
        printf("Error reading input.\n");
        return;
    }

    // Check if the account number exists
    int accountFound = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].accountNumber, accountNumber) == 0) {
            accountFound = 1;

            // Input and validate new PIN
            while (1) {
                printf("Enter new PIN (%d digits): ", PIN_LENGTH);
                if (fgets(newPin, sizeof(newPin), stdin) != NULL) {
                    newPin[strcspn(newPin, "\n")] = '\0'; // Remove newline character
                } else {
                    printf("Error reading input.\n");
                    continue;
                }

                if (strlen(newPin) == PIN_LENGTH && strspn(newPin, "0123456789") == PIN_LENGTH) {
                    printf("Confirm new PIN: ");
                    if (fgets(confirmPin, sizeof(confirmPin), stdin) != NULL) {
                        confirmPin[strcspn(confirmPin, "\n")] = '\0'; // Remove newline character
                    } else {
                        printf("Error reading input.\n");
                        continue;
                    }

                    if (strcmp(newPin, confirmPin) == 0) {
                        // Update PIN
                        strcpy(users[i].accountPin, newPin);
                        printf("PIN updated successfully.\n");
                        fileSave(users, userCount); // Save changes to file
                        return;
                    } else {
                        printf("PINs do not match. Please try again.\n");
                    }
                } else {
                    printf("Invalid PIN. Please enter exactly %d digits.\n", PIN_LENGTH);
                }
            }
        }
    }

    if (!accountFound) {
        printf("Account number not found.\n");
    }
}

void inMenuUI(struct userAccount account)          // Display inner menu with the main ATM features (Display balance, withdrawal, transfer, change pin, EXIT) 
{
    char accountNumber[ACCOUNT_NUMBER_LENGTH + 1];

    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Account Number: %s\n", account.accountNumber);
    printf("Account Name:   %s\n", account.userName);
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("1. Check Account Balance\n");
    printf("2. Withdrawal\n");
    printf("3. Transfer\n");
    printf("4. Change PIN\n");
    printf("5. Exit\n");   
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
}

long long getNextTransactionID() {
    static long long lastTransactionID = 0; // To hold the last transaction ID
    FILE *file2;
    
    // Open the file to read the last transaction ID
    file2 = fopen("transactionid.dat", "rb");
    if (file2) {
        fread(&lastTransactionID, sizeof(lastTransactionID), 1, file2);
        fclose(file2);
    }
    
    // Increment the transaction ID
    lastTransactionID++;
    
    // Open the file to write the new transaction ID
    file2 = fopen("transactionid.dat", "wb");
    if (file2) {
        fwrite(&lastTransactionID, sizeof(lastTransactionID), 1, file2);
        fclose(file2);
    } else {
        perror("Error opening transaction ID file");
        exit(EXIT_FAILURE);
    }
    
    return lastTransactionID;
}

void saveTransaction(struct userTransaction *transaction, const char *userName, const char *accountNumber, long long amount) {
    FILE *file1 = fopen("transaction-record.dat", "a+");
    if (file1 == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    char censoredAccount[ACCOUNT_NUMBER_LENGTH + 1];

    // Censor the middle 6 digits of the account number
    strncpy(censoredAccount, accountNumber, 4); // Copy first 4 digits
    for (int i = 4; i < 10; i++) 
    {
        censoredAccount[i] = 'X'; // Censor middle 6 digits
    }
    strncpy(censoredAccount + 10, accountNumber + 10, 4); // Copy last 4 digits
    censoredAccount[ACCOUNT_NUMBER_LENGTH] = '\0';

    // Get a new transaction ID
    long long transactionID = getNextTransactionID();

    // Set transaction details
    strcpy(transaction -> userName, userName);
    strcpy(transaction -> censoredAccount, censoredAccount);
    snprintf(transaction -> transactionID, sizeof(transaction -> transactionID), "%010lld", transactionID);
    transaction -> amount = amount;

    // Save the transaction to file
    fwrite(transaction, sizeof(struct userTransaction), 1, file1);

    // Increment transaction ID for next transaction
    transactionID++;

    fclose(file1);
}

void balDispUI(struct userAccount account)         // Balance display UI
{
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Account Balance\n");
     for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Account Balance: %lld\n", account.accountBalance);
}

void balWithUI(struct userAccount *account, struct userAccount users[], int userCount, struct userTransaction * transaction)         // Balance withdrawal UI (6 premade options and a customizable option)
{
    int option;
    long long amount;

    // Get the current time
    time_t now;
    struct tm *tm_info;
    char dateBuffer[80];
    char timeBuffer[80];

    time(&now); // Get current time
    tm_info = localtime(&now); // Convert to local time

    // Format the date and time
    strftime(transaction -> dateBuffer, sizeof(transaction -> dateBuffer), "%Y-%m-%d", tm_info);
    strftime(transaction -> timeBuffer, sizeof(transaction -> timeBuffer), "%H:%M:%S", tm_info);

    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Withdrawal");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Select an amount to withdraw:\n");
    printf("1. 100000\n");
    printf("2. 200000\n");
    printf("3. 500000\n");
    printf("4. 1000000\n");
    printf("5. 2000000\n");
    printf("6. Other\n");
    printf("7. Back\n");
    
    printf("Please choose an option: ");
    if (scanf("%d", &option) != 1) 
    {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();  

    switch (option) 
    {
        case 1: 
            amount = 100000; 
            break;

        case 2: 
            amount = 200000; 
            break;

        case 3: 
            amount = 500000; 
            break;

        case 4: 
            amount = 1000000; 
            break;

        case 5: 
            amount = 2000000; 
            break;

        case 6:
            printf("Enter an amount to withdraw: ");
            scanf("%lld", &amount);
            clearInputBuffer();  
            break;

        case 7:
            return;  // Return to menu

        default:
            printf("Invalid option. Please try again.\n");
            return;
    }

    if (amount + COST + VAT <= account->accountBalance) 
    {
        account->accountBalance -= amount + COST + VAT;
        printf("Withdrawal successful! New balance: %lld\n", account->accountBalance);

        // Copy userName from account to transaction data
        strcpy(transaction->userName, account->userName); 

        // Save the transaction
        saveTransaction(transaction, account->userName, account->accountNumber, amount);

        // Save the new balance to data file
        fileSave(users, userCount); 

        // Prompt to print the receipt
        char response;
        printf("Would you like to print the receipt? (Y/N): ");
        while (1) 
        {
            response = getchar();
            response = tolower(response);
            clearInputBuffer();
            
            if (response == 'y') 
            {
                // Print the Receipt
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("=");
                }
                printf("\n");
                printf("            VTC ACADEMY BANK             \n");
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("=");
                }
                printf("\n");
                printf("           Withdrawal Receipt            \n");
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("-");
                }
                printf("\n");
                printf("Date:%s            Time:%s             \n", transaction -> dateBuffer, transaction -> timeBuffer);
                printf("Account Number:           %s           \n", transaction -> censoredAccount);
                printf("TransactionID:                %s       \n", transaction -> transactionID);
                printf("Transaction Amount:               %lld \n", transaction -> amount);
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("-");
                }
                printf("\n");
                printf("Fee                                 %d  \n", COST);
                printf("VAT                                  %d \n", VAT);
                printf("Current Balance:               %lld     \n", account->accountBalance);
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("-");
                }
                printf("\n");
                printf("     Thank you for using our service!      \n");
                for (int i = 0; i < FORMAT; i++)
                {
                    printf("-");
                }
                printf("\n\n");
                break;
            } 
            else if (response == 'n') 
                {
                    break;
                } 
            else 
            {
                printf("Invalid input. Please enter 'y' for yes or 'n' for no: ");
            }
        }
    } 
        else 
        {
        printf("Insufficient funds. Please try a different amount.\n");
        }
}

void balTransUI(struct userAccount *account, struct userAccount users[], int userCount, struct userTransaction * transaction)        // Balance Transfer UI (disp account number, account balance, input destination account number, input tranfer amount  -> display remaining balance)
{
    char destAccountNumber[ACCOUNT_NUMBER_LENGTH + 1];
    char transactionID[TRANSACTION_ID_LENGTH + 1];
    long long amount;
    int found;

    // Get the current time
    time_t now;
    struct tm *tm_info;
    char dateBuffer[80];
    char timeBuffer[80];

    time(&now); // Get current time
    tm_info = localtime(&now); // Convert to local time

    // Format the date and time
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", tm_info);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", tm_info);

    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Transfer\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Enter recipient account number: ");
    fgets(destAccountNumber, sizeof(destAccountNumber), stdin);
    destAccountNumber[strcspn(destAccountNumber, "\n")] = '\0';  // Remove newline character

    printf("Enter amount to transfer: ");
    if (scanf("%lld", &amount) != 1 || amount < 0) // Cannot input anything other than positive int
    {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();  // Clear input buffer

    // Transfering process (decrease user balance, increase dest balance)
    if (amount + COST + VAT > account->accountBalance) 
    {
        printf("Insufficient balance.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].accountNumber, destAccountNumber) == 0) {
            account->accountBalance -= amount + COST + VAT;
            users[i].accountBalance += amount;
            found = 1;
            printf("Transfer successful! New balance: %lld\n", account->accountBalance);

            // Save the transaction for both the sender and the recipient
            saveTransaction(transaction, account->userName, account->accountNumber, amount);
            saveTransaction(transaction, users[i].userName, users[i].accountNumber, amount);

            // Save the new user and recipient balance to data file
            fileSave(users, userCount); 

            // Prompt to print the receipt
            char response;
            printf("Would you like to print the receipt? (Y/N): ");
            while (1) 
            {
                response = getchar();
                response = tolower(response);
                clearInputBuffer();
                
                if (response == 'y') 
                {
                    // Print the Receipt
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("=");
                    }
                    printf("\n");
                    printf("            VTC ACADEMY BANK             \n");
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("=");
                    }
                    printf("\n");
                    printf("           Transfer Receipt            \n");
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("Date:%s            Time:%s             \n", transaction -> dateBuffer, transaction -> timeBuffer);
                    printf("Account Number:           %s           \n", transaction -> censoredAccount);
                    printf("Destination Account:       %s          \n", users[i].userName);
                    printf("TransactionID:                %s       \n", transaction -> transactionID);
                    printf("Transaction Amount:               %lld \n", transaction -> amount);
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("Fee                                 %d  \n", COST);
                    printf("VAT                                  %d \n", VAT);
                    printf("Current Balance:               %lld     \n", account->accountBalance);
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("     Thank you for using our service!      \n");
                    for (int i = 0; i < FORMAT; i++)
                    {
                        printf("-");
                    }
                    printf("\n\n");
                    break;
                } 
                else if (response == 'n') 
                    {
                        break;
                    } 
                else 
                {
                    printf("Invalid input. Please enter 'y' for yes or 'n' for no: ");
                }
            }

            break;
        }
    }

    if (!found) {
        printf("Target account not found.\n");
    }
}

void chgPinUI(struct userAccount *account, struct userAccount users[], int userCount)          // Change pin UI (input account number, new pin, confirm new pin, change succesfully, EXIT)
{
    char accountNumber[15];
    char newPin[PIN_LENGTH + 1], confirmPin[PIN_LENGTH + 1];
    int found = 0;

    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("VTC ACADEMY BANK\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("=");
    }
    printf("\n");
    printf("ATM Machine \n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("Change PIN\n");
    for (int i = 0; i < FORMAT; i++)
    {
        printf("-");
    }
    printf("\n");

    // Get account number
    printf("Enter your account number: ");
    if (scanf("%14s", accountNumber) != 1) {
        printf("Invalid input. Please try again.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Search for account
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].accountNumber, accountNumber) == 0) 
        {
            found = 1;
            printf("Enter a new PIN (6 digits): ");
            fgets(newPin, sizeof(newPin), stdin);
            newPin[strcspn(newPin, "\n")] = '\0'; // Remove newline character

            if (strlen(newPin) == PIN_LENGTH && strspn(newPin, "0123456789") == PIN_LENGTH) 
            {
                printf("Confirm your new PIN: ");
                fgets(confirmPin, sizeof(confirmPin), stdin);
                confirmPin[strcspn(confirmPin, "\n")] = '\0'; // Remove newline character

                // Update PIN
                if (strcmp(newPin, confirmPin) == 0) 
                {
                    strcpy(users[i].accountPin, newPin); 
                    printf("PIN updated successfully.\n");
                    fileSave(users, userCount); //save the new PIN to data file
                } 
                    else 
                    {
                        printf("PINs do not match. Please try again.\n");
                    }
            } 
                else 
                {
                    printf("Invalid PIN number. Please enter exactly %d digits.\n", PIN_LENGTH);
                }
            break;
        }
    }

    if (!found) 
    {
        printf("Account number not found.\n");
    }
}

void confirmExit() 
{
    char choice;
    printf("Are you sure you want to exit? (Y/N): ");
    choice = getchar();
    clearInputBuffer();
    if (choice == 'Y' || choice == 'y') 
    {
        printf("Exiting program...\n");
        exit(0);
    }
    printf("Returning to menu...\n");
}

void clearInputBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}
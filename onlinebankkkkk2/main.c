#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "accounts.dat"

// Define a structure for account information
typedef struct {
    int accountNumber;
    char name[100];
    float balance;
} Account;

// Function declarations
void account();
void transferMoney();
void checkBalance();
void login();
void saveAccount(Account acc);
Account* loadAccount(int accountNumber);

int main() {
    int choice;

    while (1) {
        printf("\nBank Account System Menu:\n");
        printf("1. Create Account\n");
        printf("2. Transfer Money\n");
        printf("3. Check Balance\n");
        printf("4. Login\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                account();
                break;
            case 2:
                transferMoney();
                break;
            case 3:
                checkBalance();
                break;
            case 4:
                login();
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Function to create a new account
void account() {
    Account acc;
    FILE *file;

    printf("Enter account number: ");
    scanf("%d", &acc.accountNumber);
    printf("Enter account holder's name: ");
    scanf(" %[^\n]", acc.name);
    printf("Enter initial balance: ");
    scanf("%f", &acc.balance);

    file = fopen(FILENAME, "ab");
    if (file == NULL) {
        printf("Unable to open file!\n");
        return;
    }

    fwrite(&acc, sizeof(Account), 1, file);
    fclose(file);

    printf("Account created successfully!\n");
}

// Function to transfer money between accounts
void transferMoney() {
    int fromAccount, toAccount;
    float amount;
    Account *accFrom, *accTo;

    printf("Enter source account number: ");
    scanf("%d", &fromAccount);
    printf("Enter destination account number: ");
    scanf("%d", &toAccount);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    accFrom = loadAccount(fromAccount);
    accTo = loadAccount(toAccount);

    if (accFrom == NULL || accTo == NULL) {
        printf("Invalid account number(s)!\n");
        free(accFrom);
        free(accTo);
        return;
    }

    if (accFrom->balance < amount) {
        printf("Insufficient balance!\n");
        free(accFrom);
        free(accTo);
        return;
    }

    accFrom->balance -= amount;
    accTo->balance += amount;

    // Save updated accounts
    saveAccount(*accFrom);
    saveAccount(*accTo);

    free(accFrom);
    free(accTo);

    printf("Transfer successful!\n");
}

// Function to check balance of an account
void checkBalance() {
    int accountNumber;
    Account *acc;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    acc = loadAccount(accountNumber);
    if (acc == NULL) {
        printf("Account not found!\n");
        return;
    }

    printf("Account holder: %s\n", acc->name);
    printf("Balance: %.2f\n", acc->balance);

    free(acc);
}

// Function to login and access account
void login() {
    int accountNumber;
    Account *acc;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    acc = loadAccount(accountNumber);
    if (acc == NULL) {
        printf("Account not found!\n");
        return;
    }

    printf("Logged in successfully!\n");
    printf("Account holder: %s\n", acc->name);
    printf("Balance: %.2f\n", acc->balance);

    free(acc);
}

// Function to save an account to file
void saveAccount(Account acc) {
    FILE *file;
    Account temp;
    FILE *tempFile;

    file = fopen(FILENAME, "rb");
    tempFile = fopen("temp.dat", "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    while (fread(&temp, sizeof(Account), 1, file)) {
        if (temp.accountNumber == acc.accountNumber) {
            fwrite(&acc, sizeof(Account), 1, tempFile);
        } else {
            fwrite(&temp, sizeof(Account), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.dat", FILENAME);
}

// Function to load an account from file
Account* loadAccount(int accountNumber) {
    FILE *file;
    Account *acc = malloc(sizeof(Account));
    Account temp;

    file = fopen(FILENAME, "rb");

    if (file == NULL) {
        printf("Error opening file!\n");
        free(acc);
        return NULL;
    }

    while (fread(&temp, sizeof(Account), 1, file)) {
        if (temp.accountNumber == accountNumber) {
            *acc = temp;
            fclose(file);
            return acc;
        }
    }

    fclose(file);
    free(acc);
    return NULL;
}

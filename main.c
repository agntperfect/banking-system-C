#include "account_create.h"

int main() {
    int choice;
    long loggedInAccount = -1;  // -1 means not logged in

    while (1) {
        if (loggedInAccount == -1) {
            printf("\n--- Main Menu ---\n");
            printf("1. Login\n");
            printf("2. Create Account\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf(" %d", &choice);

            switch (choice) {
                case 1:
                    loggedInAccount = login();
                    break;
                case 2:
                    createAccount();
                    break;
                case 3:
                    printf("Exiting system...\n");
                    return 0;
                default:
                    printf("Invalid choice!\n");
            }
        } else {
            showBankingMenu();
            scanf(" %d", &choice);

            switch (choice) {
                case 1:
                    depositMoney(loggedInAccount);
                    break;
                case 2:
                    withdrawMoney(loggedInAccount);
                    break;
                case 3:
                    checkBalance(loggedInAccount);
                    break;
                case 4:
                    getDetails(loggedInAccount);
                    break;
                case 5:
                    printf("Logging out...\n");
                    loggedInAccount = -1;
                    break;
                default:
                    printf("Invalid choice!\n");
            }
        }
        pause_program();
    }
    return 0;
}
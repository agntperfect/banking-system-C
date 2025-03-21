#ifdef __unix__
#include <termios.h>
#include <unistd.h>
void pause_program() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
#elif defined(_WIN32) || defined(WIN32)
#include <conio.h>
void pause_program() {
    _getch(); // Use _getch() for Windows
}
#endif
#include "account_create.h"

void showBankingMenu() {
    printf("\n--- Banking Menu ---\n");
    printf("1. Deposit Money\n");
    printf("2. Withdraw Money\n");
    printf("3. Check Balance\n");
    printf("4. View Account Details\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");
}
void createAccount() {
    Account acc;
    FILE *file;
    
    // Generate account number FIRST
    acc.accNumber = generate_account_number();
    
    printf("Enter Name: ");
    scanf(" %49[^\n]", acc.name);
    
    printf("Enter Password: ");
    scanf(" %49s", acc.password);
    
    // Date validation loop
    int valid_date = 0;
    while(!valid_date) {
        printf("Enter Date of Birth (DD/MM/YYYY): ");
        scanf(" %d/%d/%d", &acc.dob.day, &acc.dob.month, &acc.dob.year);
        valid_date = is_valid_date(acc.dob.year, acc.dob.month, acc.dob.day);
        if(!valid_date) printf("Invalid date! Please try again.\n");
    }
    
    printf("Enter Phone no.: ");
    scanf(" %19s", acc.phone);
    
    printf("Enter Email: ");
    scanf(" %49s", acc.email);
    
    printf("Enter Citizenship Number: ");
    scanf(" %ld", &acc.citizenNumber);
    
    printf("Enter PAN number: ");
    scanf(" %19s", acc.pan);
    
    printf("Enter Address Details:\n");
    printf("Country: ");
    scanf(" %49[^\n]", acc.addr.country);
    
    printf("Province/State (Enter No.): ");
    scanf("%d", &acc.addr.province);
    
    printf("City/VDC: ");
    scanf(" %49[^\n]", acc.addr.city);
    
    printf("Ward no.: ");
    scanf("%d", &acc.addr.ward);
    
    printf("Street: ");
    scanf(" %49[^\n]", acc.addr.street);
    
    acc.balance = 0;

    // Write to file
    file = fopen(FILENAME, "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&acc, sizeof(Account), 1, file);
    fclose(file);
    
    printf("Account created successfully!\n");
    printf("Account Number: %ld\n", acc.accNumber);
    pause_program();

}

void depositMoney(long accNumber) {
    float amount;
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("Current Balance: %.2f\n", acc.balance);
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            
            acc.balance += amount;
            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, file);
            
            printf("Deposit successful! New Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    
    if (!found) printf("Account not found!\n");
    fclose(file);
}

void checkBalance(long accNumber) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("Error opening file!\n");
        pause_program();
        return;
    }

    Account acc;
    int found = 0;
    
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("\nAccount Holder: %s\n", acc.name);
            printf("Current Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found in records!\n");
    }

    fclose(file);
    pause_program();
}

void withdrawMoney(long accNumber) {
    float amount;
    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("Error opening file!\n");
        pause_program();
        return;
    }

    Account acc;
    int found = 0;
    
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("Current Balance: $%.2f\n", acc.balance);
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount!\n");
            } else if (amount > acc.balance) {
                printf("Insufficient funds!\n");
            } else {
                acc.balance -= amount;
                fseek(file, -sizeof(Account), SEEK_CUR);
                if (fwrite(&acc, sizeof(Account), 1, file) != 1) {
                    printf("Error updating record!\n");
                } else {
                    printf("Withdrawn $%.2f\nNew Balance: $%.2f\n", amount, acc.balance);
                }
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found!\n");
    }

    fclose(file);
    pause_program();
}

void getDetails(long accNumber) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("Error opening file!\n");
        pause_program();
        return;
    }

    Account acc;
    int found = 0;
    
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accNumber == accNumber) {
            printf("\n=== Account Details ===\n");
            printf("Name: %s\n", acc.name);
            printf("DOB: %02d/%02d/%d\n", acc.dob.day, acc.dob.month, acc.dob.year);
            printf("Phone: %s\n", acc.phone);
            printf("Email: %s\n", acc.email);
            printf("Citizenship No.: %ld\n", acc.citizenNumber);
            printf("PAN: %s\n", acc.pan);  // Assuming PAN is stored as string
            printf("Address: %s, %s, Ward %d, %s\n", 
                  acc.addr.street, acc.addr.city, acc.addr.ward, acc.addr.country);
            printf("Current Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found!\n");
    }

    fclose(file);
    pause_program();
}

int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


int is_valid_date(int year, int month, int day) {
    if (year < 1 || month < 1 || month > 12 || day < 1) return 0;

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(year)) days_in_month[1] = 29; // February adjustment for leap years

    return day <= days_in_month[month - 1];
}
long generate_account_number() {
    long last_number = 1000; // Default starting number if file doesn't exist

    // Read the last used account number from file
    FILE *file = fopen(ACCOUNT_NUM_FILE, "r");
    if (file != NULL) {
        fscanf(file, "%ld", &last_number);
        fclose(file);
    }

    // Increment and save the new number
    last_number++;
    file = fopen(ACCOUNT_NUM_FILE, "w");
    if (file == NULL) {
        perror("Error saving account number");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%ld", last_number);
    fclose(file);

    return last_number;
}
long login() {
    long accNumber;
    char password[50];
    
    printf("\n--- Login ---\n");
    printf("Enter Account Number: ");
    scanf("%ld", &accNumber);
    printf("Enter Password: ");
    scanf("%49s", password);

    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("Error opening accounts file!\n");
        return -1;
    }

    Account acc;
    int found = 0;
    while (fread(&acc, sizeof(Account), 1, file)) {
        if (acc.accNumber == accNumber && strcmp(acc.password, password) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        printf("Login successful!\n");
        return accNumber;
    } else {
        printf("Invalid credentials!\n");
        return -1;
    }
}

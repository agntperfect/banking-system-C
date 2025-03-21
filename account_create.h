#ifndef ACCOUNT_CREATE_H
#define ACCOUNT_CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FILENAME "accounts.dat"
#define ACCOUNT_NUM_FILE "last_acc_number.dat"

// Structure to store Date
struct Date {
    int year;
    int month;
    int day;
};

// Structure to store address 
struct Address {
    char country[50];
    int province;
    char city[50];
    int ward;
    char street[50];
};

// Structure to store account details 
typedef struct {
    char name[50];
    char password[50];  
    struct Date dob;    
    char phone[20];
    char email[50];
    long citizenNumber;
    long accNumber;
    char pan[20];       
    struct Address addr;
    float balance;
} Account;

// Function declarations
long login();
void pause_program(); 
void showBankingMenu();
void createAccount();
void getDetails(long accNumber);
void depositMoney(long accNumber);
void checkBalance(long accNumber);
void withdrawMoney(long accNumber);
long generate_account_number();
int is_leap_year(int year);
int is_valid_date(int year, int month, int day);

#endif
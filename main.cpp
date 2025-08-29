#include <bits/stdc++.h>
using namespace std;

class Account {
private:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    // Constructor
    Account(string number, string name, double initialBalance) : accountNumber(number), accountHolderName(name), balance(initialBalance) {}

    // Getter methods
    string getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolderName() const {
        return accountHolderName;
    }

    double getBalance() const {
        return balance;
    }

    // Methods for CRUD operations
    virtual void deposit(double amount) {
        balance += amount;
        cout << "Deposit successful. New balance: $" << balance << endl;
    }

    virtual void withdraw(double amount) {

        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful. New balance: $" << balance << endl;
        } else {
            cout << "Insufficient funds!" << endl;
        }
    }

    virtual void displayDetails() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: $" << balance << endl;
    }

    // Destructor (virtual to ensure proper cleanup in derived classes)
    virtual ~Account() {}
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    // Constructor
    SavingsAccount(string number, string name, double initialBalance, double rate)
        : Account(number, name, initialBalance), interestRate(rate) {}

    // Overriding the deposit method of the base class
    void deposit(double amount) override {
        // Add interest before depositing
        double interest = getBalance() * interestRate / 100.0;
        Account::deposit(interest);
        // Deposit the original amount
        Account::deposit(amount);
    }

    // Additional method for calculating interest
    void calculateInterest() {
        double interest = getBalance() * interestRate / 100.0;
        Account::deposit(interest);
        cout << "Interest calculated and added to the account. New balance: $" << getBalance() << endl;
    }

    // Overriding the displayDetails method of the base class
    void displayDetails() const override {
        Account::displayDetails();
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

class Bank {
private:
    vector<Account*> accounts;

public:
    // Method to check if an account with a given account number already exists
    bool accountExists(const string& accountNumber) const {

        for (const auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return true;
            }
        }

        return false;
    }

    // Method to add a new account
    void addAccount(Account* account) {

        if (!accountExists(account->getAccountNumber())) {
            accounts.push_back(account);
            cout << "Account added successfully." << endl;
        } else {
            cout << "Account with the same account number already exists. Cannot add duplicate account." << endl;
        }
    }

    // Method to display details of all accounts
    void displayAllAccounts() const {

        cout << "---- All Accounts ----" << endl;

        for (const auto& account : accounts) {
            account->displayDetails();  // Polymorphic call
            cout << "-----------------------" << endl;
        }
    }

    // Method to find an account by account number
    Account* findAccount(const string& accountNumber) {

        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }

        return nullptr; // Account not found
    }

    // Method to remove an account by account number
    void removeAccount(const string& accountNumber) {

        auto it = accounts.begin();

        while (it != accounts.end()) {

            if ((*it)->getAccountNumber() == accountNumber) {
                delete *it; // Release memory allocated for the account
                it = accounts.erase(it);
                cout << "Account removed successfully." << endl;
                return;
            } else {
                ++it;
            }
        }

        cout << "Account not found." << endl;
    }

    // Destructor to release memory allocated for accounts
    ~Bank() {

        for (auto& account : accounts) {
            delete account;
        }

        accounts.clear();
    }
};

void printMenu() {
    cout << "1. Add Regular Account\n";
    cout << "2. Add Savings Account\n";
    cout << "3. Display All Accounts\n";
    cout << "4. Deposit\n";
    cout << "5. Withdraw\n";
    cout << "6. Remove Account\n";
    cout << "7. Exit\n";
}

int main() {

    Bank bank;

    while (true) {

        printMenu();

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1: {

                string accNumber, accHolderName;
                double initialBalance;

                cout << "Enter Account Number: ";
                cin >> accNumber;

                if (!bank.accountExists(accNumber)) {

                    cout << "Enter Account Holder Name: ";
                    cin.ignore();
                    getline(cin, accHolderName);

                    cout << "Enter Initial Balance: ";
                    cin >> initialBalance;

                    Account* newAccount = new Account(accNumber, accHolderName, initialBalance);
                    bank.addAccount(newAccount);
                } else {
                    cout << "Account with the same account number already exists. Cannot add duplicate account." << endl;
                }

                break;
            }

            case 2: {

                string accNumber, accHolderName;
                double initialBalance, interestRate;

                cout << "Enter Account Number: ";
                cin >> accNumber;

                if (!bank.accountExists(accNumber)) {

                    cout << "Enter Account Holder Name: ";
                    cin.ignore();
                    getline(cin, accHolderName);

                    cout << "Enter Initial Balance: ";
                    cin >> initialBalance;

                    cout << "Enter Interest Rate: ";
                    cin >> interestRate;

                    SavingsAccount* newSavingsAccount = new SavingsAccount(accNumber, accHolderName, initialBalance, interestRate);
                    bank.addAccount(newSavingsAccount);
                } else {
                    cout << "Account with the same account number already exists. Cannot add duplicate account." << endl;
                }

                break;
            }

            case 3:
                bank.displayAllAccounts();
                break;

            case 4: {

                string accNumber;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accNumber;

                Account* foundAccount = bank.findAccount(accNumber);

                if (foundAccount) {
                    cout << "Enter Deposit Amount: ";
                    cin >> amount;
                    foundAccount->deposit(amount);
                } else {
                    cout << "Account not found." << endl;
                }

                break;
            }

            case 5: {

                string accNumber;
                double amount;

                cout << "Enter Account Number: ";
                cin >> accNumber;

                Account* foundAccount = bank.findAccount(accNumber);

                if (foundAccount) {
                    cout << "Enter Withdrawal Amount: ";
                    cin >> amount;
                    foundAccount->withdraw(amount);
                } else {
                    cout << "Account not found." << endl;
                }

                break;
            }

            case 6: {

                string accNumber;
                cout << "Enter Account Number to Remove: ";
                cin >> accNumber;

                bank.removeAccount(accNumber);
                break;
            }

            case 7:
                cout << "Exiting program. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
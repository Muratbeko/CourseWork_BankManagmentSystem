#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

const int MAX_ACCOUNTS = 100;

class Product {
private:
    string productName;
    string productType;
public:
    Product(string name, string type) : productName(name), productType(type) {}
    string getProductName() const { return productName; }
    string getProductType() const { return productType; }
    void setProductName(string name) { productName = name; }
    void setProductType(string type) { productType = type; }
};

class Account : public Product {
protected:
    string number;
    double balance;
public:
    Account(string num, double bal, string name, string type) : Product(name, type), number(num), balance(bal) {}
    virtual void deposit(double amount) {
        balance += amount;
    }
    virtual bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
    string getNumber() const { return number; }
    virtual double getBalance() const { return balance; }
    virtual void calculateInterest() = 0;
};

class SavingsAccount : public Account {
private:
    double interestRate;
public:
    SavingsAccount(string num, double bal, double rate, string name, string type)
        : Account(num, bal, name, type), interestRate(rate) {}
    void calculateInterest() override {
        balance += (balance * interestRate / 100);
    }
};

class CheckingAccount : public Account {
public:
    CheckingAccount(string num, double bal, string name, string type)
        : Account(num, bal, name, type) {}
    void calculateInterest() override {}
};

class LoanAccount : public Account {
private:
    double interestRate;
public:
    LoanAccount(string num, double bal, double rate, string name, string type)
        : Account(num, bal, name, type), interestRate(rate) {}

    double getInterestRate() const {
        return interestRate;
    }

    void calculateInterest() override {
        balance += (balance * interestRate / 100);
    }

    bool payLoan(double amount) {
        if (amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

class Transaction {
public:
    string from;
    string to;
    double amount;

    Transaction() : from(""), to(""), amount(0.0) {}

    Transaction(string from_, string to_, double amt)
        : from(from_), to(to_), amount(amt) {}
};

class TransactionHistory {
private:
    Transaction transactions[MAX_ACCOUNTS];
    int numTransactions;
public:
    TransactionHistory() : numTransactions(0) {}

    void addTransaction(string from_, string to_, double amount) {
        if (numTransactions < MAX_ACCOUNTS) {
            transactions[numTransactions++] = Transaction(from_, to_, amount);
        }
    }

    void printTransactions() const {
        for (int i = 0; i < numTransactions; ++i) {
            cout << "From: " << transactions[i].from
                << ", To: " << transactions[i].to
                << ", Amount: " << transactions[i].amount << endl;
        }
    }

    int getNumTransactions() const {
        return numTransactions;
    }

    Transaction getTransaction(int index) const {
        if (index >= 0 && index < numTransactions) {
            return transactions[index];
        }
        return Transaction();
    }
};

class Customer {
private:
    string name;
    string address;
    string contact;
public:
    Customer() : name(""), address(""), contact("") {}
    Customer(string n, string addr, string cont) : name(n), address(addr), contact(cont) {}
};

class Branch {
private:
    string name;
    string location;
public:
    Branch() : name(""), location("") {}
    Branch(string n, string loc) : name(n), location(loc) {}
};

class Loan {
private:
    string accountNumber;
    double amount;
    double interestRate;
    int termMonths;
    time_t startDate;
public:
    Loan(string accNum, double amt, double rate, int term, time_t start)
        : accountNumber(accNum), amount(amt), interestRate(rate), termMonths(term), startDate(start) {}

    string getAccountNumber() const { return accountNumber; }
    double getAmount() const { return amount; }
    double getInterestRate() const { return interestRate; }
    int getTermMonths() const { return termMonths; }
    time_t getStartDate() const { return startDate; }
};

class LoanPayment {
private:
    double amount;
    time_t paymentDate;
    string loanAccountNumber;
public:
    LoanPayment(double amt, time_t date, string loanAccNum)
        : amount(amt), paymentDate(date), loanAccountNumber(loanAccNum) {}

    double getAmount() const { return amount; }
    time_t getPaymentDate() const { return paymentDate; }
    string getLoanAccountNumber() const { return loanAccountNumber; }
};

class LoanManager {
private:
    vector<Loan> loans;
    vector<LoanPayment> loanPayments;
public:
    void addLoan(const Loan& loan) {
        loans.push_back(loan);
    }

    void makeLoanPayment(const LoanPayment& payment) {
        loanPayments.push_back(payment);
    }
};

class Bank {
private:
    Account* accounts[MAX_ACCOUNTS];
    TransactionHistory transactionHistory;
    Customer customers[MAX_ACCOUNTS];
    Branch branches[MAX_ACCOUNTS];
    LoanManager loanManager;
    int numAccounts;
    int numCustomers;
    int numBranches;

    void writeAccountsToFile() {
        ofstream outFile("accounts.txt");
        if (outFile.is_open()) {
            for (int i = 0; i < numAccounts; ++i) {
                outFile << accounts[i]->getNumber() << " " << accounts[i]->getBalance() << endl;
            }
            outFile.close();
        }
        else {
            cout << "Unable to open file for writing accounts." << endl;
        }
    }

    void writeTransactionsToFile() {
        ofstream outFile("transactions.txt");
        if (outFile.is_open()) {
            for (int i = 0; i < transactionHistory.getNumTransactions(); ++i) {
                Transaction t = transactionHistory.getTransaction(i);
                outFile << t.from << " " << t.to << " " << t.amount << endl;
            }
            outFile.close();
        }
        else {
            cout << "Unable to open file for writing transactions." << endl;
        }
    }

public:
    Bank() : numAccounts(0), numCustomers(0), numBranches(0) {}

    void addAccount(Account* account) {
        if (numAccounts < MAX_ACCOUNTS) {
            accounts[numAccounts++] = account;
            writeAccountsToFile();
        }
    }

    void createAccount() {
        int accountType;
        string accNum;
        double initialDeposit;
        cout << "Select account type:\n1. Savings\n2. Checking\n3. Loan\n";
        cin >> accountType;
        cout << "Enter account number: ";
        cin >> accNum;
        cout << "Enter initial deposit: ";
        cin >> initialDeposit;
        switch (accountType) {
        case 1: {
            double interestRate;
            cout << "Enter interest rate: ";
            cin >> interestRate;
            Account* acc = new SavingsAccount(accNum, initialDeposit, interestRate, "Savings Account", "Savings");
            addAccount(acc);
            break;
        }
        case 2: {
            Account* acc = new CheckingAccount(accNum, initialDeposit, "Checking Account", "Checking");
            addAccount(acc);
            break;
        }
        case 3: {
            double interestRate;
            cout << "Enter loan interest rate: ";
            cin >> interestRate;
            Account* acc = new LoanAccount(accNum, initialDeposit, interestRate, "Loan Account", "Loan");
            addAccount(acc);
            break;
        }
        default:
            cout << "Invalid account type.\n";
        }
        cout << "Account created.\n";
    }

    void executeTransaction() {
        string fromAcc, toAcc;
        double amount;
        cout << "Enter sender account number: ";
        cin >> fromAcc;
        cout << "Enter recipient account number: ";
        cin >> toAcc;
        cout << "Enter transfer amount: ";
        cin >> amount;
        if (transferMoney(fromAcc, toAcc, amount)) {
            cout << "Transfer successful.\n";
        }
        else {
            cout << "Error performing transfer.\n";
        }
    }

    bool transferMoney(const string& fromAcc, const string& toAcc, double amount) {
        Account* fromAccount = findAccount(fromAcc);
        Account* toAccount = findAccount(toAcc);
        if (fromAccount && toAccount && fromAccount->withdraw(amount)) {
            toAccount->deposit(amount);
            transactionHistory.addTransaction(fromAcc, toAcc, amount);
            writeTransactionsToFile();
            return true;
        }
        return false;
    }

    Account* findAccount(const string& accNum) {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getNumber() == accNum) {
                return accounts[i];
            }
        }
        return nullptr;
    }

    void printAccountDetails(const string& accNum) const {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getNumber() == accNum) {
                cout << "Account Number: " << accounts[i]->getNumber() << endl;
                cout << "Balance: " << accounts[i]->getBalance() << endl;
            }
        }
    }

    void printTransactionHistory() const {
        transactionHistory.printTransactions();
    }
};

int main() {
    Bank bank;
    bank.createAccount();
    bank.executeTransaction();
    bank.printTransactionHistory();
    return 0;
}

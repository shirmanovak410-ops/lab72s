#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

// Класс "Банковский кредит"
class BankCredit {
private:
    std::string name;          // название
    double amount;             // сумма кредита
    std::string currency;      // тип валюты
    double interestRate;       // ставка в % годовых

public:
    BankCredit() : name(""), amount(0.0), currency(""), interestRate(0.0) {}
    BankCredit(const std::string& n, double a, const std::string& c, double r)
        : name(n), amount(a), currency(c), interestRate(r) {}
    BankCredit(const BankCredit& other) = default;
    BankCredit(BankCredit&& other) noexcept = default;
    BankCredit& operator=(const BankCredit& other) = default;
    BankCredit& operator=(BankCredit&& other) noexcept = default;
    ~BankCredit() = default;
    std::string getName() const { return name; }
    double getAmount() const { return amount; }
    std::string getCurrency() const { return currency; }
    double getInterestRate() const { return interestRate; }
    bool operator<(const BankCredit& other) const {
        return this->interestRate < other.interestRate;
    }
    friend std::ostream& operator<<(std::ostream& os, const BankCredit& cr) {
        os << cr.name << " | " << cr.amount << " " << cr.currency
           << " | " << cr.interestRate << "%";
        return os;
    }
};

int main() {
    std::ifstream infile("input.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening input.txt" << std::endl;
        return 1;
    }
    std::list<BankCredit> credits;
    std::string name, currency;
    double amount, rate;
    while (std::getline(infile, name)) {
        if (name.empty()) continue; // пропуск пустых строк
        infile >> amount;
        infile.ignore();
        std::getline(infile, currency);
        infile >> rate;
        infile.ignore();

        credits.emplace_back(name, amount, currency, rate);
    }
    infile.close();
    std::ofstream outfile("output.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error opening output.txt" << std::endl;
        return 1;
    }

    outfile << "Original container (list)\n";
    for (const auto& cr : credits) {
        outfile << cr << "\n";
    }
    outfile << "\n";
    credits.sort();  // list::sort без параметров

    outfile << "Container after sorting by interest rate (%%)\n";
    for (const auto& cr : credits) {
        outfile << cr << "\n";
    }
    outfile << "\n";
    std::vector<BankCredit> creditsVec;
    creditsVec.resize(credits.size());
    std::copy(credits.begin(), credits.end(), creditsVec.begin());

    outfile << "Copied container (vector)\n";
    for (const auto& cr : creditsVec) {
        outfile << cr << "\n";
    }

    outfile.close();

    std::cout << "Done. Results saved to output.txt" << std::endl;
    return 0;
}
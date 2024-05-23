#include "Transaction.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include "Account.h"

namespace {
// RAII
struct Guard {
    Guard(Account& account) : account_(&account) { account_->Lock(); }
    ~Guard() { account_->Unlock(); }

private:
    Account* account_;
};
}  // namespace

Transaction::Transaction() : fee_(1) {}

Transaction::~Transaction() {}

bool Transaction::Make(Account& from, Account& to, int sum) {
    if (from.id() == to.id()) throw std::logic_error("invalid action");
    if (sum < 0) throw std::invalid_argument("sum can't be negative");
    if (sum < 100) throw std::logic_error("too small");
    if (fee_ * 2 > sum) return false;

    Guard guard_from(from);
    Guard guard_to(to);

    std::cout << "Initial Balances: from=" << from.GetBalance() << " to=" << to.GetBalance() << std::endl;

    Credit(to, sum);

    std::cout << "After Credit: from=" << from.GetBalance() << " to=" << to.GetBalance() << std::endl;

    bool success = Debit(from, sum + fee_);
    if (!success) {
        to.ChangeBalance(-sum);
    }

    std::cout << "After Debit: from=" << from.GetBalance() << " to=" << to.GetBalance() << std::endl;

    SaveToDataBase(from, to, sum);
    return success;
}

void Transaction::Credit(Account& account, int sum) {
    assert(sum > 0);
    account.ChangeBalance(sum);
}

bool Transaction::Debit(Account& account, int sum) {
    assert(sum > 0);
    if (account.GetBalance() >= sum) {
        account.ChangeBalance(-sum);
        return true;
    }
    return false;
}

void Transaction::SaveToDataBase(Account& from, Account& to, int sum) {
    std::cout << from.id() << " send to " << to.id() << " $" << sum << std::endl;
    std::cout << "Balance " << from.id() << " is " << from.GetBalance() << std::endl;
    std::cout << "Balance " << to.id() << " is " << to.GetBalance() << std::endl;
}

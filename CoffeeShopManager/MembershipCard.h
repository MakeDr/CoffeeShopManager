#pragma once

class MembershipCard {
private:
    int deposit;

public:
    MembershipCard(int initialDeposit = 0) : deposit(initialDeposit) {}

    int getDeposit() const { return deposit; }
    void charge(int amount) { deposit += amount; }
    bool pay(int amount) {
        if (deposit >= amount) {
            deposit -= amount;
            return true;
        }
        return false;
    }
};
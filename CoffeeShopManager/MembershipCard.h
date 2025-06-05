#pragma once

class MembershipCard {
private:
    int deposit; // 선불카드에 충전된 금액 (잔액)

public:
    // 기본값 0으로 설정
    MembershipCard(int initialDeposit = 0) : deposit(initialDeposit) {}

    int getDeposit() const { return deposit; }

    // 돈 충전하는 함수
    void charge(int amount) {
        deposit += amount;
    }

    // 돈 쓰는 함수. 잔액이 충분해야만 결제 성공(true)
    bool pay(int amount) {
        if (deposit >= amount) {
            deposit -= amount;
            return true; // 결제 성공!
        }
        return false; // 돈 부족 그지..
    }
};
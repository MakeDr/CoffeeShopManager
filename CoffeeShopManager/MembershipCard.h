#pragma once

class MembershipCard {
private:
    int deposit; // ����ī�忡 ������ �ݾ� (�ܾ�)

public:
    // ī�带 ó�� ���� �� �ܾ��� 0���̰���? �⺻�� 0���� ����
    MembershipCard(int initialDeposit = 0) : deposit(initialDeposit) {}

    int getDeposit() const { return deposit; }

    // �� �����ϴ� �Լ�
    void charge(int amount) {
        deposit += amount;
    }

    // �� ���� �Լ�. �ܾ��� ����ؾ߸� ���� ����(true)
    bool pay(int amount) {
        if (deposit >= amount) {
            deposit -= amount;
            return true; // ���� ����!
        }
        return false; // �� ���� �Ф�
    }
};
#pragma once
#define _CRT_SECURE_NO_WARNINGS // localtime ���� ��� ����
#include <string>
#include "MembershipCard.h"

class Member {
private:
    int id;
    std::string name;
    std::string phone;
    int points;
    MembershipCard card;

public:
    Member(int id, std::string name, std::string phone, int points = 0, int initialDeposit = 0)
        : id(id), name(name), phone(phone), points(points), card(initialDeposit) {
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    int getPoints() const { return points; }

    void addPoints(int amount) { points += amount; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }

    // non-const ����
    MembershipCard& getCard() { return card; }
    // const ���� (�����ε�)
    const MembershipCard& getCard() const { return card; }

    void display() const {
        std::cout << "ȸ����ȣ: " << id << "\t�̸�: " << name << "\t����ó: " << phone
            << "\t����Ʈ: " << points << "\t�ܾ�: " << card.getDeposit() << "��" << std::endl;
    }
};
#pragma once
#define _CRT_SECURE_NO_WARNINGS // localtime 보안 경고 무시
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

    // non-const 버전
    MembershipCard& getCard() { return card; }
    // const 버전 (오버로딩)
    const MembershipCard& getCard() const { return card; }

    void display() const {
        std::cout << "회원번호: " << id << "\t이름: " << name << "\t연락처: " << phone
            << "\t포인트: " << points << "\t잔액: " << card.getDeposit() << "원" << std::endl;
    }
};
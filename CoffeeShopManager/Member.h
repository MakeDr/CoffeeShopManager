#pragma once
#include <string>
#include <iostream>
#include "MembershipCard.h" // Member가 MembershipCard를 쓰니까 포함시켜야 함

class Member {
private:
    int id; // 회원 번호
    std::string name; // 회원 이름
    std::string phone; // 연락처 (나중에 이걸로 회원 찾을 거임)
    int points; // 포인트!

    // 핵심 : Member 객체 안에 MembershipCard 객체가 통째로 들어있음
    // Composition 관계 : 회원이 소멸하면 카드도 같이 소멸됨.
    MembershipCard card;

public:
    Member(int id, std::string name, std::string phone, int points = 0, int initialDeposit = 0)
        // : card(initialDeposit) -> 멤버 객체 초기화
        // Member 생성자가 불릴 때, 자기가 가진 card 객체의 생성자도 불러줌.
        : id(id), name(name), phone(phone), points(points), card(initialDeposit) {
    }

    // Getter 함수들
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    int getPoints() const { return points; }

    // 포인트 쌓기
    void addPoints(int amount) {
        points += amount;
    }

    // 카드를 직접 건드리는 게 아니라, 카드에 접근할 수 있는 통로를 열어줌
    // 'const'가 안 붙은 이유는, 이 함수를 통해 카드 잔액을 바꿀 수도 있어서그럼.
    MembershipCard& getCard() {
        return card;
    }

    // 회원 정보 출력
    void display() const {
        std::cout << "회원번호: " << id << "\t이름: " << name << "\t연락처: " << phone
            << "\t포인트: " << points
            << "\t잔액: " << card.getDeposit() << "원" << std::endl;
    }
};
#pragma once
#include <string>
#include <iostream>
#include "MembershipCard.h" // Member�� MembershipCard�� ���ϱ� ���Խ��Ѿ� ��

class Member {
private:
    int id; // ȸ�� ��ȣ
    std::string name; // ȸ�� �̸�
    std::string phone; // ����ó (���߿� �̰ɷ� ȸ�� ã�� ����)
    int points; // ����Ʈ!

    // �ٽ� : Member ��ü �ȿ� MembershipCard ��ü�� ��°�� �������
    // Composition ���� : ȸ���� �Ҹ��ϸ� ī�嵵 ���� �Ҹ��.
    MembershipCard card;

public:
    Member(int id, std::string name, std::string phone, int points = 0, int initialDeposit = 0)
        // : card(initialDeposit) -> ��� ��ü �ʱ�ȭ
        // Member �����ڰ� �Ҹ� ��, �ڱⰡ ���� card ��ü�� �����ڵ� �ҷ���.
        : id(id), name(name), phone(phone), points(points), card(initialDeposit) {
    }

    // Getter �Լ���
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    int getPoints() const { return points; }

    // ����Ʈ �ױ�
    void addPoints(int amount) {
        points += amount;
    }

    // ī�带 ���� �ǵ帮�� �� �ƴ϶�, ī�忡 ������ �� �ִ� ��θ� ������
    // 'const'�� �� ���� ������, �� �Լ��� ���� ī�� �ܾ��� �ٲ� ���� �־�׷�.
    MembershipCard& getCard() {
        return card;
    }

    // ȸ�� ���� ���
    void display() const {
        std::cout << "ȸ����ȣ: " << id << "\t�̸�: " << name << "\t����ó: " << phone
            << "\t����Ʈ: " << points
            << "\t�ܾ�: " << card.getDeposit() << "��" << std::endl;
    }
};
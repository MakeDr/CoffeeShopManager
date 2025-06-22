#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <memory> // ����Ʈ ������ ������ ����
#include <ctime>  // �ֹ� �ð� ��Ͽ�

#include "Product.h"
#include "Member.h"
#include "FileHandler.h" // 2������ ���� ���� �ڵ鷯
#include "Order.h"       // 3������ ���׷��̵��� �ֹ� Ŭ����

class ShopManager {
private:
    // ===== ��� ����: �� Ŭ������ �����ϴ� ��� ������ =====
    std::vector<Product> products; // �츮 ������ ��� ��ǰ ���
    std::vector<Member> members;   // �츮 ������ ��� ȸ�� ���
    std::vector<Order> orders;     // ��� �ֹ� ��� (���� ���� �ε�� �� ��)
    FileHandler fileHandler;       // ���� ó�� ������
    int nextOrderId = 1;           // ������ ������ �ֹ��� ID (���α׷� �� ������ 1�� ����)

    // ===== �̱��� ������ ���� private ������ =====
    // �ۿ��� new ShopManager()�� ���ϰ� ���� ����. "������� �� ���̸� �����!"
    ShopManager() {}

    // ===== �ǽð� ������ ���� private �Լ��� =====
    // �ۿ����� �� �Լ��� ���� �θ� �ʿ� ����, �ֹ� ó�� ���� ���� �������� �˾Ƽ� ȣ����

    // [ȸ�� ���� ����]
    // ����� ī�� ����, ����Ʈ ���� �� ȸ�� ������ ����� ������ ȣ���
    void saveMembers() {
        std::vector<std::vector<std::string>> memberData;
        // ���� ù �ٿ� ���(�� �̸�)�� ����� ���߿� �˾ƺ��� ����
        memberData.push_back({ "id", "name", "phone", "points", "deposit" });

        for (auto& m : members) {
            // Member ��ü �ϳ��� ���ڿ� �� ��(����)�� ��ȯ
            memberData.push_back({
                std::to_string(m.getId()),
                m.getName(),
                m.getPhone(),
                std::to_string(m.getPoints()),
                std::to_string(m.getCard().getDeposit())
                });
        }
        // �غ�� 2���� �����͸� ���Ͽ� �ѹ濡 �����
        fileHandler.save("members.csv", memberData);
        std::cout << ">> (System) ȸ�� ������ ���Ͽ� ����Ǿ����ϴ�." << std::endl;
    }

    // [�ֹ� ���� ����]
    // �̰� ���� ������ ����� �� �ǰ�, ��� '�߰�(append)'�ؾ� �ֹ� ����� ����
    void appendOrder(const Order& order) {
        // std::ios::app -> �̰� �ٷ� append ���� ������ ���� ������ Ű����
        std::ofstream file("orders.csv", std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error: orders.csv ������ �� �� �����ϴ�." << std::endl;
            return;
        }

        // �ֹ� ����: orderId,memberId,productId,quantity,totalPrice
        for (const auto& item : order.getItems()) {
            file << order.getOrderId() << ","
                << order.getMemberId() << ","
                << item.first << ","  // ��ǰ ID
                << item.second << "," // ����
                << order.getTotalPrice() // �� �ֹ��� �Ѿ�
                << "\n";
        }
        file.close();
        std::cout << ">> (System) �ֹ� ����� ���Ͽ� �߰��Ǿ����ϴ�." << std::endl;
    }

public:
    // ===== �̱��� ������ ���� public ������ =====
    // ���� �����ڿ� ���� �����ڸ� ���Ƽ� ���ϼ��� ����
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;

    // ���α׷� ��𼭵� 'ShopManager::getInstance()'�� �θ��� �Ȱ��� ��ü�� ���� �� ����
    static ShopManager& getInstance() {
        static ShopManager instance;
        return instance;
    }

    // ===== ������ �ε� �Լ� =====
    // ���α׷� ���� �� �� �� �� ȣ���
    void loadData() {
        // --- ��ǰ ������ �ε� ---
        auto productData = fileHandler.load("products.csv");
        if (productData.size() > 1) { // ����� �ִ� ���� ������ �ƴ��� Ȯ��
            for (size_t i = 1; i < productData.size(); ++i) { // 0�� ��(���)�� �ǳʶ�
                int id = std::stoi(productData[i][0]);
                std::string name = productData[i][1];
                int price = std::stoi(productData[i][2]);
                products.emplace_back(id, name, price);
            }
        }

        // --- ȸ�� ������ �ε� ---
        auto memberData = fileHandler.load("members.csv");
        if (memberData.size() > 1) {
            for (size_t i = 1; i < memberData.size(); ++i) {
                int id = std::stoi(memberData[i][0]);
                std::string name = memberData[i][1];
                std::string phone = memberData[i][2];
                int points = std::stoi(memberData[i][3]);
                int deposit = std::stoi(memberData[i][4]);
                members.emplace_back(id, name, phone, points, deposit);
            }
        }
        // �ֹ� ������(orders.csv)�� ���� ���� �� �־, ������ �� �ε��ϴ� �� �ϴ� ����!
        // ���߿� '���� ���' ���� ��� ���� �� �ʿ��ϸ� �ε�����.
    }

    // ===== �˻�(Find) ���� �Լ��� =====
    // ID�� ��ȭ��ȣ�� ��ü�� ���� ã�� �����ִ� �Լ���
    Product* findProductById(int id) {
        for (auto& p : products) {
            if (p.getId() == id) return &p;
        }
        return nullptr;
    }

    Member* findMemberByPhone(const std::string& phone) {
        for (auto& m : members) {
            if (m.getPhone() == phone) return &m;
        }
        return nullptr;
    }

    // ===== UI ���� �Լ� =====
    // �ֿܼ� �޴��� �����ִ� ���
    void showProducts() const {
        std::cout << "\n----------- KOREATECH CAFE �޴��� -----------" << std::endl;
        for (const auto& p : products) {
            p.display();
        }
        std::cout << "---------------------------------------------" << std::endl;
    }

    // ===== �ٽ� ����Ͻ� ����: �ֹ� ó�� =====
    // 3������ ����! �ֹ����� ����, ������� ��� ������ ó��
    void processOrder() {
        showProducts();

        // 1. �ֹ� ��ü ���� �� ��ٱ��� ���
        Order newOrder(nextOrderId); // ���� �ֹ���ȣ�� �� �ֹ��� ����

        while (true) {
            std::cout << "��ǰ ID�� �Է��ϼ��� (�ֹ� �Ϸ�� 0): ";
            int productId;
            std::cin >> productId;

            if (std::cin.fail()) { // ���ڰ� �ƴ� �̻��Ѱ� �Է����� �� ���
                std::cout << ">> �߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << std::endl;
                std::cin.clear(); // ���� ���� �ʱ�ȭ
                std::cin.ignore(10000, '\n'); // �Է� ���� ����
                continue;
            }

            if (productId == 0) break;

            Product* p = findProductById(productId);
            if (p == nullptr) {
                std::cout << ">> �������� �ʴ� ��ǰ ID�Դϴ�." << std::endl;
                continue;
            }

            std::cout << "������ �Է��ϼ���: ";
            int quantity;
            std::cin >> quantity;
            if (quantity > 0) {
                newOrder.addItem(productId, quantity);
                std::cout << ">> " << p->getName() << " " << quantity << "�� ��ٱ��Ͽ� ��ҽ��ϴ�!" << std::endl;
            }
        }

        if (newOrder.getItems().empty()) {
            std::cout << ">> ��ٱ��ϰ� ����ֽ��ϴ�. �ֹ��� ����մϴ�." << std::endl;
            return;
        }

        // 2. �� �ݾ� ���
        int totalPrice = 0;
        for (const auto& item : newOrder.getItems()) {
            totalPrice += findProductById(item.first)->getPrice() * item.second;
        }
        newOrder.setTotalPrice(totalPrice);
        std::cout << "\n>> �� �ֹ� �ݾ��� " << totalPrice << "�� �Դϴ�." << std::endl;

        // 3. ȸ�� Ȯ�� �� ���� ó��
        std::cout << "ȸ���̽Ű���? (y/n): ";
        char isMember;
        std::cin >> isMember;

        Member* currentMember = nullptr; // ���� �ֹ��ϴ� ȸ���� ����ų ������
        if (isMember == 'y' || isMember == 'Y') {
            std::cout << "�޴��� ��ȣ�� �Է��ϼ���: ";
            std::string phone;
            std::cin >> phone;
            currentMember = findMemberByPhone(phone);

            if (currentMember != nullptr) {
                std::cout << ">> " << currentMember->getName() << "��, �ȳ��ϼ���! (����Ʈ: "
                    << currentMember->getPoints() << "��, �ܾ�: "
                    << currentMember->getCard().getDeposit() << "��)" << std::endl;
            }
            else {
                std::cout << ">> ȸ�� ������ �����ϴ�. ��ȸ������ ������ �����մϴ�." << std::endl;
            }
        }

        // 4. ���� ���� �� ������ ����
        std::cout << "\n������ �����մϴ�..." << std::endl;
        if (currentMember != nullptr) { // ȸ���� ���
            std::cout << "���� ������ �����ϼ��� (1: �Ϲݰ���, 2: ����� ī��): ";
            int choice;
            std::cin >> choice;

            if (choice == 2 && currentMember->getCard().pay(totalPrice)) {
                std::cout << ">> ����� ī��� ���� ����!" << std::endl;
            }
            else {
                if (choice == 2) std::cout << ">> �ܾ��� �����Ͽ� �Ϲݰ����� �����մϴ�." << std::endl;
                std::cout << ">> �Ϲݰ��� ����!" << std::endl;
            }
            // ����Ʈ ����
            int earnedPoints = static_cast<int>(totalPrice * 0.05);
            currentMember->addPoints(earnedPoints);
            std::cout << ">> " << earnedPoints << " ����Ʈ �����Ǿ����ϴ�!" << std::endl;

            // �߿�! ȸ�� ������ �ٲ������ ��� ���Ͽ� ����
            saveMembers();
        }
        else { // ��ȸ���� ���
            std::cout << ">> �Ϲݰ��� ����!" << std::endl;
        }

        // �ֹ��� ���������� �������� �ֹ� ��ϵ� ���Ͽ� �߰�
        appendOrder(newOrder);
        // ���α׷� �޸𸮿��� �ֹ� ��� ����
        orders.push_back(newOrder);
        nextOrderId++; // ���� �ֹ��� ���� �ֹ� ��ȣ 1 ����

        std::cout << "\n>> �ֹ��� ���������� �Ϸ�Ǿ����ϴ�. �����մϴ�! <<\n" << std::endl;
    }
};
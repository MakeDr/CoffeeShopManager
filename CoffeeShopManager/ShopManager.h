#pragma once
#include "Product.h"
#include "Member.h"
#include "Order.h"
#include "FileHandler.h"
#include <vector>
#include <string>
#include <memory>
#include <limits> // �Է� ���� ��� �� ���

class ShopManager {
private:
    std::vector<Product> products;
    std::vector<Member> members;
    std::vector<Order> orders;
    FileHandler fileHandler;
    int nextProductId = 1;
    int nextMemberId = 1;
    int nextOrderId = 1;

    ShopManager() {}

    // ������ ���Ͽ� �ǽð� �����ϴ� �Լ���
    void saveProducts() {
        std::vector<std::vector<std::string>> productData;
        productData.push_back({ "id", "name", "price", "category" });
        for (const auto& p : products) {
            productData.push_back({ std::to_string(p.getId()), p.getName(), std::to_string(p.getPrice()), p.getCategory() });
        }
        fileHandler.save("data/products.csv", productData);
        std::cout << ">> (System) ��ǰ ������ ���Ͽ� ����Ǿ����ϴ�." << std::endl;
    }

    void saveMembers() {
        std::vector<std::vector<std::string>> memberData;
        memberData.push_back({ "id", "name", "phone", "points", "deposit" });
        for (const auto& m : members) {
            memberData.push_back({ std::to_string(m.getId()), m.getName(), m.getPhone(), std::to_string(m.getPoints()), std::to_string(m.getCard().getDeposit()) });
        }
        fileHandler.save("data/members.csv", memberData);
        std::cout << ">> (System) ȸ�� ������ ���Ͽ� ����Ǿ����ϴ�." << std::endl;
    }

    void appendOrder(const Order& order) {
        std::ofstream file("data/orders.csv", std::ios::app);
        if (!file.is_open()) return;
        for (const auto& item : order.getItems()) {
            file << order.getOrderId() << "," << order.getMemberId() << "," << item.first << "," << item.second << "," << order.getTotalPrice() << "\n";
        }
        file.close();
    }

    // ������ �Է� ���� ó�� �Լ�
    int getIntegerInput() {
        int value;
        while (!(std::cin >> value)) {
            std::cout << ">> �߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ����Ű ����
        return value;
    }


public:
    // �̱��� ����
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;
    static ShopManager& getInstance() {
        static ShopManager instance;
        return instance;
    }

    void loadData(); // ������ cpp ���Ϸ� �и�

    // �˻� �Լ���
    Product* findProductById(int id);
    Member* findMemberByPhone(const std::string& phone);

    // ��庰 ��� �Լ���
    void runAdminMode();
    void runStaffMode();

    // ������ ���
    void manageProducts();
    void manageMembers();

    // ���� ���
    void processOrder();
    void chargeMembershipCard();
};
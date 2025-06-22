#pragma once
#include "Product.h"
#include "Member.h"
#include "Order.h"
#include "FileHandler.h"
#include <vector>
#include <string>
#include <memory>
#include <limits> // 입력 버퍼 비울 때 사용

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

    // 데이터 파일에 실시간 저장하는 함수들
    void saveProducts() {
        std::vector<std::vector<std::string>> productData;
        productData.push_back({ "id", "name", "price", "category" });
        for (const auto& p : products) {
            productData.push_back({ std::to_string(p.getId()), p.getName(), std::to_string(p.getPrice()), p.getCategory() });
        }
        fileHandler.save("data/products.csv", productData);
        std::cout << ">> (System) 상품 정보가 파일에 저장되었습니다." << std::endl;
    }

    void saveMembers() {
        std::vector<std::vector<std::string>> memberData;
        memberData.push_back({ "id", "name", "phone", "points", "deposit" });
        for (const auto& m : members) {
            memberData.push_back({ std::to_string(m.getId()), m.getName(), m.getPhone(), std::to_string(m.getPoints()), std::to_string(m.getCard().getDeposit()) });
        }
        fileHandler.save("data/members.csv", memberData);
        std::cout << ">> (System) 회원 정보가 파일에 저장되었습니다." << std::endl;
    }

    void appendOrder(const Order& order) {
        std::ofstream file("data/orders.csv", std::ios::app);
        if (!file.is_open()) return;
        for (const auto& item : order.getItems()) {
            file << order.getOrderId() << "," << order.getMemberId() << "," << item.first << "," << item.second << "," << order.getTotalPrice() << "\n";
        }
        file.close();
    }

    // 간단한 입력 예외 처리 함수
    int getIntegerInput() {
        int value;
        while (!(std::cin >> value)) {
            std::cout << ">> 잘못된 입력입니다. 숫자만 입력해주세요: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 엔터키 비우기
        return value;
    }


public:
    // 싱글톤 패턴
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;
    static ShopManager& getInstance() {
        static ShopManager instance;
        return instance;
    }

    void loadData(); // 구현은 cpp 파일로 분리

    // 검색 함수들
    Product* findProductById(int id);
    Member* findMemberByPhone(const std::string& phone);

    // 모드별 기능 함수들
    void runAdminMode();
    void runStaffMode();

    // 관리자 기능
    void manageProducts();
    void manageMembers();

    // 직원 기능
    void processOrder();
    void chargeMembershipCard();
};
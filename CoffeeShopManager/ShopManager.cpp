#include "ShopManager.h"
#include <iostream>

void ShopManager::loadData() {
    auto productData = fileHandler.load("data/products.csv");
    int maxProductId = 0;
    if (productData.size() > 1) {
        for (size_t i = 1; i < productData.size(); ++i) {
            int id = std::stoi(productData[i][0]);
            if (id > maxProductId) maxProductId = id;
            products.emplace_back(id, productData[i][1], std::stoi(productData[i][2]), productData[i][3]);
        }
    }
    nextProductId = maxProductId + 1;

    auto memberData = fileHandler.load("data/members.csv");
    int maxMemberId = 0;
    if (memberData.size() > 1) {
        for (size_t i = 1; i < memberData.size(); ++i) {
            int id = std::stoi(memberData[i][0]);
            if (id > maxMemberId) maxMemberId = id;
            members.emplace_back(id, memberData[i][1], memberData[i][2], std::stoi(memberData[i][3]), std::stoi(memberData[i][4]));
        }
    }
    nextMemberId = maxMemberId + 1;

    // 주문 데이터는 양이 많을 수 있어, 일단 마지막 주문 ID만 확인
    auto orderData = fileHandler.load("data/orders.csv");
    int maxOrderId = 0;
    if (!orderData.empty()) {
        for (const auto& row : orderData) {
            if (!row.empty()) {
                int id = std::stoi(row[0]);
                if (id > maxOrderId) maxOrderId = id;
            }
        }
    }
    nextOrderId = maxOrderId + 1;
}

Product* ShopManager::findProductById(int id) {
    for (auto& p : products) if (p.getId() == id) return &p;
    return nullptr;
}

Member* ShopManager::findMemberByPhone(const std::string& phone) {
    for (auto& m : members) if (m.getPhone() == phone) return &m;
    return nullptr;
}

void ShopManager::runAdminMode() {
    while (true) {
        std::cout << "\n--- 관리자 모드 ---" << std::endl;
        std::cout << "1. 상품 관리" << std::endl;
        std::cout << "2. 회원 관리" << std::endl;
        std::cout << "3. 이전 메뉴로" << std::endl;
        std::cout << ">> 선택: ";
        int choice = getIntegerInput();
        if (choice == 1) manageProducts();
        else if (choice == 2) manageMembers();
        else if (choice == 3) break;
        else std::cout << ">> 잘못된 선택입니다." << std::endl;
    }
}

void ShopManager::manageProducts() {
    std::cout << "\n--- 상품 목록 ---" << std::endl;
    for (const auto& p : products) p.display();
    std::cout << "-----------------" << std::endl;
    std::cout << "1. 상품 추가  2. 상품 수정  3. 돌아가기: ";
    int choice = getIntegerInput();

    if (choice == 1) {
        std::string name, category;
        int price;
        std::cout << "새 상품 이름: "; std::getline(std::cin, name);
        std::cout << "새 상품 가격: "; price = getIntegerInput();
        std::cout << "새 상품 분류: "; std::getline(std::cin, category);
        products.emplace_back(nextProductId++, name, price, category);
        saveProducts();
    }
    else if (choice == 2) {
        std::cout << "수정할 상품 ID: ";
        int id = getIntegerInput();
        Product* p = findProductById(id);
        if (p) {
            std::cout << "새 이름 (기존: " << p->getName() << "): ";
            std::string newName; std::getline(std::cin, newName);
            std::cout << "새 가격 (기존: " << p->getPrice() << "): ";
            int newPrice = getIntegerInput();
            p->setName(newName);
            p->setPrice(newPrice);
            saveProducts();
        }
        else {
            std::cout << ">> 상품을 찾을 수 없습니다." << std::endl;
        }
    }
}

void ShopManager::manageMembers() {
    std::cout << "\n--- 회원 목록 ---" << std::endl;
    for (const auto& m : members) m.display();
    std::cout << "-----------------" << std::endl;
    std::cout << "1. 회원 추가  2. 회원 수정  3. 돌아가기: ";
    int choice = getIntegerInput();

    if (choice == 1) {
        std::string name, phone;
        std::cout << "새 회원 이름: "; std::getline(std::cin, name);
        std::cout << "새 회원 연락처: "; std::getline(std::cin, phone);
        members.emplace_back(nextMemberId++, name, phone);
        saveMembers();
    }
    // 회원 수정 기능은 유사하게 구현 가능 (생략)
}


void ShopManager::runStaffMode() {
    while (true) {
        std::cout << "\n--- 직원 모드 ---" << std::endl;
        std::cout << "1. 주문 처리" << std::endl;
        std::cout << "2. 멤버십 카드 충전" << std::endl;
        std::cout << "3. 이전 메뉴로" << std::endl;
        std::cout << ">> 선택: ";
        int choice = getIntegerInput();
        if (choice == 1) processOrder();
        else if (choice == 2) chargeMembershipCard();
        else if (choice == 3) break;
        else std::cout << ">> 잘못된 선택입니다." << std::endl;
    }
}

void ShopManager::processOrder() {
    std::cout << "\n--- 메뉴판 ---" << std::endl;
    for (const auto& p : products) p.display();
    std::cout << "--------------" << std::endl;

    Order newOrder(nextOrderId);
    while (true) {
        std::cout << "상품 ID (주문 완료: 0): ";
        int id = getIntegerInput();
        if (id == 0) break;
        if (findProductById(id)) {
            std::cout << "수량: ";
            int quantity = getIntegerInput();
            if (quantity > 0) newOrder.addItem(id, quantity);
        }
        else {
            std::cout << ">> 없는 상품입니다." << std::endl;
        }
    }

    if (newOrder.getItems().empty()) {
        std::cout << ">> 주문이 취소되었습니다." << std::endl;
        return;
    }

    int totalPrice = 0;
    for (const auto& item : newOrder.getItems()) {
        totalPrice += findProductById(item.first)->getPrice() * item.second;
    }
    newOrder.setTotalPrice(totalPrice);
    std::cout << "\n>> 총 주문 금액: " << totalPrice << "원" << std::endl;

    std::cout << "회원 주문인가요? (y/n): ";
    char isMember;
    std::cin >> isMember;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Member* member = nullptr;
    if (isMember == 'y' || isMember == 'Y') {
        std::cout << "회원 연락처: ";
        std::string phone;
        std::getline(std::cin, phone);
        member = findMemberByPhone(phone);
        if (!member) std::cout << ">> 없는 회원입니다. 비회원으로 진행합니다." << std::endl;
    }

    if (member) { // 회원 결제
        std::cout << ">> " << member->getName() << "님, 결제를 진행합니다." << std::endl;
        std::cout << "1. 일반결제  2. 멤버십 카드: ";
        int choice = getIntegerInput();
        if (choice == 2 && !member->getCard().pay(totalPrice)) {
            std::cout << ">> 잔액 부족! 일반결제로 진행합니다." << std::endl;
        }
        int earnedPoints = static_cast<int>(totalPrice * 0.05);
        member->addPoints(earnedPoints);
        std::cout << ">> " << earnedPoints << "포인트 적립 완료!" << std::endl;
        saveMembers();
    }

    appendOrder(newOrder);
    orders.push_back(newOrder);
    nextOrderId++;
    std::cout << "\n>> 주문이 완료되었습니다. 감사합니다! <<\n" << std::endl;
}

void ShopManager::chargeMembershipCard() {
    std::cout << "충전할 회원의 연락처: ";
    std::string phone;
    std::getline(std::cin, phone);
    Member* member = findMemberByPhone(phone);
    if (member) {
        std::cout << "충전할 금액: ";
        int amount = getIntegerInput();
        if (amount > 0) {
            member->getCard().charge(amount);
            std::cout << ">> " << amount << "원 충전 완료! (현재 잔액: " << member->getCard().getDeposit() << "원)" << std::endl;
            saveMembers();
        }
    }
    else {
        std::cout << ">> 없는 회원입니다." << std::endl;
    }
}
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

    // �ֹ� �����ʹ� ���� ���� �� �־�, �ϴ� ������ �ֹ� ID�� Ȯ��
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
        std::cout << "\n--- ������ ��� ---" << std::endl;
        std::cout << "1. ��ǰ ����" << std::endl;
        std::cout << "2. ȸ�� ����" << std::endl;
        std::cout << "3. ���� �޴���" << std::endl;
        std::cout << ">> ����: ";
        int choice = getIntegerInput();
        if (choice == 1) manageProducts();
        else if (choice == 2) manageMembers();
        else if (choice == 3) break;
        else std::cout << ">> �߸��� �����Դϴ�." << std::endl;
    }
}

void ShopManager::manageProducts() {
    std::cout << "\n--- ��ǰ ��� ---" << std::endl;
    for (const auto& p : products) p.display();
    std::cout << "-----------------" << std::endl;
    std::cout << "1. ��ǰ �߰�  2. ��ǰ ����  3. ���ư���: ";
    int choice = getIntegerInput();

    if (choice == 1) {
        std::string name, category;
        int price;
        std::cout << "�� ��ǰ �̸�: "; std::getline(std::cin, name);
        std::cout << "�� ��ǰ ����: "; price = getIntegerInput();
        std::cout << "�� ��ǰ �з�: "; std::getline(std::cin, category);
        products.emplace_back(nextProductId++, name, price, category);
        saveProducts();
    }
    else if (choice == 2) {
        std::cout << "������ ��ǰ ID: ";
        int id = getIntegerInput();
        Product* p = findProductById(id);
        if (p) {
            std::cout << "�� �̸� (����: " << p->getName() << "): ";
            std::string newName; std::getline(std::cin, newName);
            std::cout << "�� ���� (����: " << p->getPrice() << "): ";
            int newPrice = getIntegerInput();
            p->setName(newName);
            p->setPrice(newPrice);
            saveProducts();
        }
        else {
            std::cout << ">> ��ǰ�� ã�� �� �����ϴ�." << std::endl;
        }
    }
}

void ShopManager::manageMembers() {
    std::cout << "\n--- ȸ�� ��� ---" << std::endl;
    for (const auto& m : members) m.display();
    std::cout << "-----------------" << std::endl;
    std::cout << "1. ȸ�� �߰�  2. ȸ�� ����  3. ���ư���: ";
    int choice = getIntegerInput();

    if (choice == 1) {
        std::string name, phone;
        std::cout << "�� ȸ�� �̸�: "; std::getline(std::cin, name);
        std::cout << "�� ȸ�� ����ó: "; std::getline(std::cin, phone);
        members.emplace_back(nextMemberId++, name, phone);
        saveMembers();
    }
    // ȸ�� ���� ����� �����ϰ� ���� ���� (����)
}


void ShopManager::runStaffMode() {
    while (true) {
        std::cout << "\n--- ���� ��� ---" << std::endl;
        std::cout << "1. �ֹ� ó��" << std::endl;
        std::cout << "2. ����� ī�� ����" << std::endl;
        std::cout << "3. ���� �޴���" << std::endl;
        std::cout << ">> ����: ";
        int choice = getIntegerInput();
        if (choice == 1) processOrder();
        else if (choice == 2) chargeMembershipCard();
        else if (choice == 3) break;
        else std::cout << ">> �߸��� �����Դϴ�." << std::endl;
    }
}

void ShopManager::processOrder() {
    std::cout << "\n--- �޴��� ---" << std::endl;
    for (const auto& p : products) p.display();
    std::cout << "--------------" << std::endl;

    Order newOrder(nextOrderId);
    while (true) {
        std::cout << "��ǰ ID (�ֹ� �Ϸ�: 0): ";
        int id = getIntegerInput();
        if (id == 0) break;
        if (findProductById(id)) {
            std::cout << "����: ";
            int quantity = getIntegerInput();
            if (quantity > 0) newOrder.addItem(id, quantity);
        }
        else {
            std::cout << ">> ���� ��ǰ�Դϴ�." << std::endl;
        }
    }

    if (newOrder.getItems().empty()) {
        std::cout << ">> �ֹ��� ��ҵǾ����ϴ�." << std::endl;
        return;
    }

    int totalPrice = 0;
    for (const auto& item : newOrder.getItems()) {
        totalPrice += findProductById(item.first)->getPrice() * item.second;
    }
    newOrder.setTotalPrice(totalPrice);
    std::cout << "\n>> �� �ֹ� �ݾ�: " << totalPrice << "��" << std::endl;

    std::cout << "ȸ�� �ֹ��ΰ���? (y/n): ";
    char isMember;
    std::cin >> isMember;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Member* member = nullptr;
    if (isMember == 'y' || isMember == 'Y') {
        std::cout << "ȸ�� ����ó: ";
        std::string phone;
        std::getline(std::cin, phone);
        member = findMemberByPhone(phone);
        if (!member) std::cout << ">> ���� ȸ���Դϴ�. ��ȸ������ �����մϴ�." << std::endl;
    }

    if (member) { // ȸ�� ����
        std::cout << ">> " << member->getName() << "��, ������ �����մϴ�." << std::endl;
        std::cout << "1. �Ϲݰ���  2. ����� ī��: ";
        int choice = getIntegerInput();
        if (choice == 2 && !member->getCard().pay(totalPrice)) {
            std::cout << ">> �ܾ� ����! �Ϲݰ����� �����մϴ�." << std::endl;
        }
        int earnedPoints = static_cast<int>(totalPrice * 0.05);
        member->addPoints(earnedPoints);
        std::cout << ">> " << earnedPoints << "����Ʈ ���� �Ϸ�!" << std::endl;
        saveMembers();
    }

    appendOrder(newOrder);
    orders.push_back(newOrder);
    nextOrderId++;
    std::cout << "\n>> �ֹ��� �Ϸ�Ǿ����ϴ�. �����մϴ�! <<\n" << std::endl;
}

void ShopManager::chargeMembershipCard() {
    std::cout << "������ ȸ���� ����ó: ";
    std::string phone;
    std::getline(std::cin, phone);
    Member* member = findMemberByPhone(phone);
    if (member) {
        std::cout << "������ �ݾ�: ";
        int amount = getIntegerInput();
        if (amount > 0) {
            member->getCard().charge(amount);
            std::cout << ">> " << amount << "�� ���� �Ϸ�! (���� �ܾ�: " << member->getCard().getDeposit() << "��)" << std::endl;
            saveMembers();
        }
    }
    else {
        std::cout << ">> ���� ȸ���Դϴ�." << std::endl;
    }
}
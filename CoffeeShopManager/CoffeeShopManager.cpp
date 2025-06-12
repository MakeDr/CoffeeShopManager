#include <iostream>
#include "ShopManager.h"

void showMainMenu() {
    std::cout << "\n===== KOREATECH 커피숍 =====" << std::endl;
    std::cout << "1. 주문하기" << std::endl;
    std::cout << "2. 프로그램 종료" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << ">> 선택: ";
}

int main() {
    ShopManager& manager = ShopManager::getInstance();
    manager.loadData();

    while (true) {
        showMainMenu();
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            manager.processOrder();
        }
        else if (choice == 2) {
            std::cout << "프로그램을 종료합니다." << std::endl;
            break;
        }
        else {
            std::cout << ">> 잘못된 입력입니다. 다시 선택해주세요." << std::endl;
        }
    }

    return 0;
}
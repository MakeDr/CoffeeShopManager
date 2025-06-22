#include <iostream>
#include "ShopManager.h"

void showMainMenu() {
    std::cout << "\n===== KOREATECH 커피숍 관리 시스템 =====" << std::endl;
    std::cout << "1. 관리자 모드" << std::endl;
    std::cout << "2. 직원 모드" << std::endl;
    std::cout << "3. 프로그램 종료" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << ">> 선택: ";
}

int main() {
    // VS 설정: 프로젝트 속성 > 디버깅 > 작업 디렉토리 -> $(ProjectDir) 로 변경 필수!
    ShopManager& manager = ShopManager::getInstance();
    manager.loadData();

    while (true) {
        showMainMenu();
        int choice;
        std::cin >> choice;

        // 간단한 입력 예외 처리
        if (std::cin.fail()) {
            std::cout << ">> 잘못된 입력입니다. 숫자만 입력해주세요." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n'); // 엔터키 입력 버퍼 비우기

        switch (choice) {
        case 1:
            manager.runAdminMode();
            break;
        case 2:
            manager.runStaffMode();
            break;
        case 3:
            std::cout << "프로그램을 종료합니다." << std::endl;
            return 0;
        default:
            std::cout << ">> 잘못된 선택입니다. 1-3 사이의 숫자를 입력해주세요." << std::endl;
            break;
        }
    }

    return 0;
}
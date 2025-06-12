#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <memory> // 스마트 포인터 쓰려고 포함
#include <ctime>  // 주문 시간 기록용

#include "Product.h"
#include "Member.h"
#include "FileHandler.h" // 2주차에 만든 파일 핸들러
#include "Order.h"       // 3주차에 업그레이드한 주문 클래스

class ShopManager {
private:
    // ===== 멤버 변수: 이 클래스가 관리하는 모든 데이터 =====
    std::vector<Product> products; // 우리 가게의 모든 상품 목록
    std::vector<Member> members;   // 우리 가게의 모든 회원 목록
    std::vector<Order> orders;     // 모든 주문 기록 (아직 파일 로드는 안 함)
    FileHandler fileHandler;       // 파일 처리 전문가
    int nextOrderId = 1;           // 다음에 생성될 주문의 ID (프로그램 켤 때마다 1로 시작)

    // ===== 싱글톤 패턴을 위한 private 생성자 =====
    // 밖에서 new ShopManager()를 못하게 막는 역할. "사장님은 한 명이면 충분해!"
    ShopManager() {}

    // ===== 실시간 저장을 위한 private 함수들 =====
    // 밖에서는 이 함수를 직접 부를 필요 없고, 주문 처리 같은 내부 로직에서 알아서 호출함

    // [회원 정보 저장]
    // 멤버십 카드 결제, 포인트 적립 등 회원 정보가 변경될 때마다 호출됨
    void saveMembers() {
        std::vector<std::vector<std::string>> memberData;
        // 파일 첫 줄에 헤더(열 이름)를 써줘야 나중에 알아보기 편함
        memberData.push_back({ "id", "name", "phone", "points", "deposit" });

        for (auto& m : members) {
            // Member 객체 하나를 문자열 한 줄(벡터)로 변환
            memberData.push_back({
                std::to_string(m.getId()),
                m.getName(),
                m.getPhone(),
                std::to_string(m.getPoints()),
                std::to_string(m.getCard().getDeposit())
                });
        }
        // 준비된 2차원 데이터를 파일에 한방에 덮어쓰기
        fileHandler.save("members.csv", memberData);
        std::cout << ">> (System) 회원 정보가 파일에 저장되었습니다." << std::endl;
    }

    // [주문 정보 저장]
    // 이건 기존 내용을 덮어쓰면 안 되고, 계속 '추가(append)'해야 주문 기록이 쌓임
    void appendOrder(const Order& order) {
        // std::ios::app -> 이게 바로 append 모드로 파일을 여는 마법의 키워드
        std::ofstream file("orders.csv", std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Error: orders.csv 파일을 열 수 없습니다." << std::endl;
            return;
        }

        // 주문 포맷: orderId,memberId,productId,quantity,totalPrice
        for (const auto& item : order.getItems()) {
            file << order.getOrderId() << ","
                << order.getMemberId() << ","
                << item.first << ","  // 상품 ID
                << item.second << "," // 수량
                << order.getTotalPrice() // 이 주문의 총액
                << "\n";
        }
        file.close();
        std::cout << ">> (System) 주문 기록이 파일에 추가되었습니다." << std::endl;
    }

public:
    // ===== 싱글톤 패턴을 위한 public 접근자 =====
    // 복사 생성자와 대입 연산자를 막아서 유일성을 보장
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;

    // 프로그램 어디서든 'ShopManager::getInstance()'를 부르면 똑같은 객체를 얻을 수 있음
    static ShopManager& getInstance() {
        static ShopManager instance;
        return instance;
    }

    // ===== 데이터 로딩 함수 =====
    // 프로그램 시작 시 딱 한 번 호출됨
    void loadData() {
        // --- 상품 데이터 로드 ---
        auto productData = fileHandler.load("products.csv");
        if (productData.size() > 1) { // 헤더만 있는 깡통 파일이 아닌지 확인
            for (size_t i = 1; i < productData.size(); ++i) { // 0번 줄(헤더)은 건너뜀
                int id = std::stoi(productData[i][0]);
                std::string name = productData[i][1];
                int price = std::stoi(productData[i][2]);
                products.emplace_back(id, name, price);
            }
        }

        // --- 회원 데이터 로드 ---
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
        // 주문 데이터(orders.csv)는 양이 많을 수 있어서, 시작할 때 로드하는 건 일단 생략!
        // 나중에 '매출 통계' 같은 기능 만들 때 필요하면 로드하자.
    }

    // ===== 검색(Find) 헬퍼 함수들 =====
    // ID나 전화번호로 객체를 쉽게 찾게 도와주는 함수들
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

    // ===== UI 관련 함수 =====
    // 콘솔에 메뉴판 보여주는 기능
    void showProducts() const {
        std::cout << "\n----------- KOREATECH CAFE 메뉴판 -----------" << std::endl;
        for (const auto& p : products) {
            p.display();
        }
        std::cout << "---------------------------------------------" << std::endl;
    }

    // ===== 핵심 비즈니스 로직: 주문 처리 =====
    // 3주차의 심장! 주문부터 결제, 저장까지 모든 과정을 처리
    void processOrder() {
        showProducts();

        // 1. 주문 객체 생성 및 장바구니 담기
        Order newOrder(nextOrderId); // 다음 주문번호로 새 주문서 생성

        while (true) {
            std::cout << "상품 ID를 입력하세요 (주문 완료는 0): ";
            int productId;
            std::cin >> productId;

            if (std::cin.fail()) { // 숫자가 아닌 이상한거 입력했을 때 방어
                std::cout << ">> 잘못된 입력입니다. 숫자만 입력해주세요." << std::endl;
                std::cin.clear(); // 에러 상태 초기화
                std::cin.ignore(10000, '\n'); // 입력 버퍼 비우기
                continue;
            }

            if (productId == 0) break;

            Product* p = findProductById(productId);
            if (p == nullptr) {
                std::cout << ">> 존재하지 않는 상품 ID입니다." << std::endl;
                continue;
            }

            std::cout << "수량을 입력하세요: ";
            int quantity;
            std::cin >> quantity;
            if (quantity > 0) {
                newOrder.addItem(productId, quantity);
                std::cout << ">> " << p->getName() << " " << quantity << "개 장바구니에 담았습니다!" << std::endl;
            }
        }

        if (newOrder.getItems().empty()) {
            std::cout << ">> 장바구니가 비어있습니다. 주문을 취소합니다." << std::endl;
            return;
        }

        // 2. 총 금액 계산
        int totalPrice = 0;
        for (const auto& item : newOrder.getItems()) {
            totalPrice += findProductById(item.first)->getPrice() * item.second;
        }
        newOrder.setTotalPrice(totalPrice);
        std::cout << "\n>> 총 주문 금액은 " << totalPrice << "원 입니다." << std::endl;

        // 3. 회원 확인 및 결제 처리
        std::cout << "회원이신가요? (y/n): ";
        char isMember;
        std::cin >> isMember;

        Member* currentMember = nullptr; // 현재 주문하는 회원을 가리킬 포인터
        if (isMember == 'y' || isMember == 'Y') {
            std::cout << "휴대폰 번호를 입력하세요: ";
            std::string phone;
            std::cin >> phone;
            currentMember = findMemberByPhone(phone);

            if (currentMember != nullptr) {
                std::cout << ">> " << currentMember->getName() << "님, 안녕하세요! (포인트: "
                    << currentMember->getPoints() << "점, 잔액: "
                    << currentMember->getCard().getDeposit() << "원)" << std::endl;
            }
            else {
                std::cout << ">> 회원 정보가 없습니다. 비회원으로 결제를 진행합니다." << std::endl;
            }
        }

        // 4. 최종 결제 및 데이터 저장
        std::cout << "\n결제를 진행합니다..." << std::endl;
        if (currentMember != nullptr) { // 회원일 경우
            std::cout << "결제 수단을 선택하세요 (1: 일반결제, 2: 멤버십 카드): ";
            int choice;
            std::cin >> choice;

            if (choice == 2 && currentMember->getCard().pay(totalPrice)) {
                std::cout << ">> 멤버십 카드로 결제 성공!" << std::endl;
            }
            else {
                if (choice == 2) std::cout << ">> 잔액이 부족하여 일반결제로 진행합니다." << std::endl;
                std::cout << ">> 일반결제 성공!" << std::endl;
            }
            // 포인트 적립
            int earnedPoints = static_cast<int>(totalPrice * 0.05);
            currentMember->addPoints(earnedPoints);
            std::cout << ">> " << earnedPoints << " 포인트 적립되었습니다!" << std::endl;

            // 중요! 회원 정보가 바뀌었으니 즉시 파일에 저장
            saveMembers();
        }
        else { // 비회원일 경우
            std::cout << ">> 일반결제 성공!" << std::endl;
        }

        // 주문이 성공적으로 끝났으니 주문 기록도 파일에 추가
        appendOrder(newOrder);
        // 프로그램 메모리에도 주문 기록 저장
        orders.push_back(newOrder);
        nextOrderId++; // 다음 주문을 위해 주문 번호 1 증가

        std::cout << "\n>> 주문이 성공적으로 완료되었습니다. 감사합니다! <<\n" << std::endl;
    }
};
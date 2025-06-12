#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <map> // 장바구니 만들 때 map이 완전 편함
#include <ctime> // 주문 시간 기록용
#include "Product.h"
#include "Member.h"

class Order {
private:
    int orderId; // 주문 번호
    // 장바구니: <상품 ID, 주문 수량> 형태로 저장. 똑같은 상품 여러 번 담아도 수량만 늘어나서 관리하기 좋아
    std::map<int, int> items;
    int memberId; // 주문한 회원의 ID. 비회원이면 0 같은 걸로 표시하자
    int totalPrice; // 최종 결제 금액
    std::string orderTime; // 주문 시간

public:
    // 비회원 주문일 수도 있으니 memberId는 기본값 0으로 설정
    Order(int orderId, int memberId = 0)
        : orderId(orderId), memberId(memberId), totalPrice(0) {
        time_t now = time(0);
        struct tm timeinfo; // 시간 정보를 저장할 구조체를 직접 만들어야 함

        // localtime_s: 시간 정보를 우리가 만든 timeinfo 구조체에 '안전하게' 복사해 줌
        localtime_s(&timeinfo, &now);

        char buf[80];
        // strftime은 이제 timeinfo의 주소를 받아서 사용
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        this->orderTime = buf;
    }

    // 장바구니에 상품 추가
    void addItem(int productId, int quantity) {
        items[productId] += quantity; // map이라서 같은 productId가 들어오면 기존 수량에 더해짐. 개꿀!
    }

    // Getter들
    int getOrderId() const { return orderId; }
    const std::map<int, int>& getItems() const { return items; }
    int getMemberId() const { return memberId; }
    int getTotalPrice() const { return totalPrice; }

    // 최종 결제 금액 설정 (계산은 ShopManager에서 하고, 여기선 값만 받아서 저장)
    void setTotalPrice(int price) {
        this->totalPrice = price;
    }

    // 주문 정보 깔끔하게 출력
    void display(const std::vector<Product>& products) const {
        std::cout << "--- 주문 번호: " << orderId << " (" << orderTime << ") ---" << std::endl;
        if (memberId != 0) {
            std::cout << "주문 회원 ID: " << memberId << std::endl;
        }
        else {
            std::cout << "주문: 비회원" << std::endl;
        }

        std::cout << " [주문 내역]" << std::endl;
        for (const auto& pair : items) {
            int productId = pair.first;
            int quantity = pair.second;
            // 상품 ID로 전체 상품 목록에서 이름이랑 가격 찾아오기
            for (const auto& p : products) {
                if (p.getId() == productId) {
                    std::cout << "  - " << p.getName() << " x " << quantity << std::endl;
                    break;
                }
            }
        }
        std::cout << " > 총 결제 금액: " << totalPrice << "원" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
};
#include <iostream>
#include "Product.h"
#include "Member.h"

int main() {
    // 1주차에 만든 클래스들이 잘 돌아가는지 테스트해보는 공간

    std::cout << "===== 상품 테스트 =====" << std::endl;
    Product americano(101, "아메리카노", 4500);
    Product latte(102, "카페라떼", 5000);
    americano.display();
    latte.display();

    std::cout << "\n===== 회원 테스트 =====" << std::endl;
    // 김코딩 회원을 만들고, 처음부터 50000원을 충전해놓자.
    Member member1(1, "김코딩", "010-1234-5678", 0, 50000);
    member1.display();

    std::cout << "\n>> 10000원 충전 후..." << std::endl;
    // member1의 카드를 '직접' 가져와서 10000원 충전!
    member1.getCard().charge(10000);
    member1.display();

    std::cout << "\n>> 라떼 한 잔 결제 후..." << std::endl;
    // 라떼 가격만큼 결제 시도
    if (member1.getCard().pay(latte.getPrice())) {
        std::cout << "결제 성공!" << std::endl;
        // 결제 성공하면 주문 금액의 5%를 포인트로 적립 (정책 미리 테스트)
        int earnedPoints = static_cast<int>(latte.getPrice() * 0.05);
        member1.addPoints(earnedPoints);
    }
    else {
        std::cout << "결제 실패... 잔액 부족" << std::endl;
    }
    member1.display();

    return 0;
}
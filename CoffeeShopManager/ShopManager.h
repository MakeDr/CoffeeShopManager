#pragma once
#include <vector>
#include <memory> // 스마트 포인터 쓰려고 포함
#include "Product.h"
#include "Member.h"
#include "FileHandler.h"

class ShopManager {
private:
    std::vector<Product> products; // 우리 가게의 모든 상품 목록
    std::vector<Member> members; // 우리 가게의 모든 회원 목록
	FileHandler fileHandler; // CSV 파일 읽고 쓰는 기능 담당

    // 싱글톤 패턴의 핵심: 생성자를 private으로 막아서 밖에서 new ShopManager() 못하게 함
    ShopManager() {}

public:
    // 싱글톤 패턴의 핵심: 복사 생성자와 대입 연산자도 막아야 완벽한 싱글톤!
    // 이렇게 안 하면 ShopManager a = b; 이런 식으로 복사본이 생길 수 있음
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;

    // 싱글톤 패턴의 핵심: 오직 이 함수를 통해서만 ShopManager 객체에 접근 가능
    static ShopManager& getInstance() {
        // static 변수는 프로그램 시작할 때 딱 한 번만 만들어지고, 계속 유지됨
        static ShopManager instance;
        return instance;
    }

    // 파일에서 모든 데이터를 읽어서 메모리(벡터)에 올리는 함수
    void loadData() {
        // --- 상품 데이터 로드 ---
        auto productData = fileHandler.load("data/products.csv");
        // 첫 줄(id,name,price)은 헤더니까 건너뛰고 1번 인덱스부터 시작
        for (size_t i = 1; i < productData.size(); ++i) {
            // stoi: string to int. 문자열을 숫자로 바꿔줌
            int id = std::stoi(productData[i][0]);
            std::string name = productData[i][1];
            int price = std::stoi(productData[i][2]);
            products.emplace_back(id, name, price); // 벡터에 새 Product 객체 추가
        }

        // --- 회원 데이터 로드 ---
        auto memberData = fileHandler.load("data/members.csv");
        for (size_t i = 1; i < memberData.size(); ++i) {
            int id = std::stoi(memberData[i][0]);
            std::string name = memberData[i][1];
            std::string phone = memberData[i][2];
            int points = std::stoi(memberData[i][3]);
            int deposit = std::stoi(memberData[i][4]);
            members.emplace_back(id, name, phone, points, deposit);
        }
    }

    // 현재 메모리에 있는 모든 객체 정보를 파일로 저장하는 함수
    void saveData() {
        // saveData는 아직 안 만들었어! 3주차에 '즉시 저장' 방식으로 더 멋지게 만들거임
        // 지금은 그냥 비움
    }

    // 그냥 테스트용으로 모든 상품, 회원 정보 출력하는 함수
    void displayAllData() {
        std::cout << "\n===== 전체 상품 목록 =====" << std::endl;
        for (const auto& p : products) {
            p.display();
        }
        std::cout << "\n===== 전체 회원 목록 =====" << std::endl;
        for (const auto& m : members) {
            m.display();
        }
    }
};
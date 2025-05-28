#pragma once // 헤더 파일 중복 포함 방지, 이거 완전 국룰임
#include <iostream>
#include <string>

class Product {
private:
    int id; // 상품 고유번호
    std::string name; // 상품 이름 (아메리카노 같은 거)
    int price; // 가격

public:
    // 생성자: 객체가 처음 만들어질 때 호출되는 애. 여기서 초기화해줌
    Product(int id, std::string name, int price) {
        this->id = id;
        this->name = name;
        this->price = price;
    }

    // private 멤버는 밖에서 직접 못 건드니까, 값을 가져갈 수 있는 통로(Getter)를 열어주는 거야
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getPrice() const { return price; }

    // 상품 정보 깔끔하게 출력해주는 함수
    void display() const {
        std::cout << "ID: " << id << "\t상품명: " << name << "\t가격: " << price << "원" << std::endl;
    }
};
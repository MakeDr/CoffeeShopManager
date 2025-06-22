#pragma once
#include <iostream>
#include <string>

class Product {
private:
    int id;
    std::string name;
    int price;
    std::string category;

public:
    Product(int id, std::string name, int price, std::string category)
        : id(id), name(name), price(price), category(category) {
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getPrice() const { return price; }
    std::string getCategory() const { return category; }

    void setName(const std::string& newName) { name = newName; }
    void setPrice(int newPrice) { price = newPrice; }

    void display() const {
        std::cout << "ID: " << id << "\t상품명: " << name << "\t가격: " << price << "원\t분류: " << category << std::endl;
    }
};
#pragma once // ��� ���� �ߺ� ���� ����, �̰� ���� ������
#include <iostream>
#include <string>

class Product {
private:
    int id; // ��ǰ ������ȣ
    std::string name; // ��ǰ �̸� (�Ƹ޸�ī�� ���� ��)
    int price; // ����

public:
    // ������: ��ü�� ó�� ������� �� ȣ��Ǵ� ��. ���⼭ �ʱ�ȭ����
    Product(int id, std::string name, int price) {
        this->id = id;
        this->name = name;
        this->price = price;
    }

    // private ����� �ۿ��� ���� �� �ǵ�ϱ�, ���� ������ �� �ִ� ���(Getter)�� �����ִ� �ž�
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getPrice() const { return price; }

    // ��ǰ ���� ����ϰ� ������ִ� �Լ�
    void display() const {
        std::cout << "ID: " << id << "\t��ǰ��: " << name << "\t����: " << price << "��" << std::endl;
    }
};
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <map> // ��ٱ��� ���� �� map�� ���� ����
#include <ctime> // �ֹ� �ð� ��Ͽ�
#include "Product.h"
#include "Member.h"

class Order {
private:
    int orderId; // �ֹ� ��ȣ
    // ��ٱ���: <��ǰ ID, �ֹ� ����> ���·� ����. �Ȱ��� ��ǰ ���� �� ��Ƶ� ������ �þ�� �����ϱ� ����
    std::map<int, int> items;
    int memberId; // �ֹ��� ȸ���� ID. ��ȸ���̸� 0 ���� �ɷ� ǥ������
    int totalPrice; // ���� ���� �ݾ�
    std::string orderTime; // �ֹ� �ð�

public:
    // ��ȸ�� �ֹ��� ���� ������ memberId�� �⺻�� 0���� ����
    Order(int orderId, int memberId = 0)
        : orderId(orderId), memberId(memberId), totalPrice(0) {
        time_t now = time(0);
        struct tm timeinfo; // �ð� ������ ������ ����ü�� ���� ������ ��

        // localtime_s: �ð� ������ �츮�� ���� timeinfo ����ü�� '�����ϰ�' ������ ��
        localtime_s(&timeinfo, &now);

        char buf[80];
        // strftime�� ���� timeinfo�� �ּҸ� �޾Ƽ� ���
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        this->orderTime = buf;
    }

    // ��ٱ��Ͽ� ��ǰ �߰�
    void addItem(int productId, int quantity) {
        items[productId] += quantity; // map�̶� ���� productId�� ������ ���� ������ ������. ����!
    }

    // Getter��
    int getOrderId() const { return orderId; }
    const std::map<int, int>& getItems() const { return items; }
    int getMemberId() const { return memberId; }
    int getTotalPrice() const { return totalPrice; }

    // ���� ���� �ݾ� ���� (����� ShopManager���� �ϰ�, ���⼱ ���� �޾Ƽ� ����)
    void setTotalPrice(int price) {
        this->totalPrice = price;
    }

    // �ֹ� ���� ����ϰ� ���
    void display(const std::vector<Product>& products) const {
        std::cout << "--- �ֹ� ��ȣ: " << orderId << " (" << orderTime << ") ---" << std::endl;
        if (memberId != 0) {
            std::cout << "�ֹ� ȸ�� ID: " << memberId << std::endl;
        }
        else {
            std::cout << "�ֹ�: ��ȸ��" << std::endl;
        }

        std::cout << " [�ֹ� ����]" << std::endl;
        for (const auto& pair : items) {
            int productId = pair.first;
            int quantity = pair.second;
            // ��ǰ ID�� ��ü ��ǰ ��Ͽ��� �̸��̶� ���� ã�ƿ���
            for (const auto& p : products) {
                if (p.getId() == productId) {
                    std::cout << "  - " << p.getName() << " x " << quantity << std::endl;
                    break;
                }
            }
        }
        std::cout << " > �� ���� �ݾ�: " << totalPrice << "��" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }
};
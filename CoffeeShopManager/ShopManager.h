#pragma once
#include <vector>
#include <memory> // ����Ʈ ������ ������ ����
#include "Product.h"
#include "Member.h"
#include "FileHandler.h"

class ShopManager {
private:
    std::vector<Product> products; // �츮 ������ ��� ��ǰ ���
    std::vector<Member> members; // �츮 ������ ��� ȸ�� ���
	FileHandler fileHandler; // CSV ���� �а� ���� ��� ���

    // �̱��� ������ �ٽ�: �����ڸ� private���� ���Ƽ� �ۿ��� new ShopManager() ���ϰ� ��
    ShopManager() {}

public:
    // �̱��� ������ �ٽ�: ���� �����ڿ� ���� �����ڵ� ���ƾ� �Ϻ��� �̱���!
    // �̷��� �� �ϸ� ShopManager a = b; �̷� ������ ���纻�� ���� �� ����
    ShopManager(const ShopManager&) = delete;
    void operator=(const ShopManager&) = delete;

    // �̱��� ������ �ٽ�: ���� �� �Լ��� ���ؼ��� ShopManager ��ü�� ���� ����
    static ShopManager& getInstance() {
        // static ������ ���α׷� ������ �� �� �� ���� ���������, ��� ������
        static ShopManager instance;
        return instance;
    }

    // ���Ͽ��� ��� �����͸� �о �޸�(����)�� �ø��� �Լ�
    void loadData() {
        // --- ��ǰ ������ �ε� ---
        auto productData = fileHandler.load("data/products.csv");
        // ù ��(id,name,price)�� ����ϱ� �ǳʶٰ� 1�� �ε������� ����
        for (size_t i = 1; i < productData.size(); ++i) {
            // stoi: string to int. ���ڿ��� ���ڷ� �ٲ���
            int id = std::stoi(productData[i][0]);
            std::string name = productData[i][1];
            int price = std::stoi(productData[i][2]);
            products.emplace_back(id, name, price); // ���Ϳ� �� Product ��ü �߰�
        }

        // --- ȸ�� ������ �ε� ---
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

    // ���� �޸𸮿� �ִ� ��� ��ü ������ ���Ϸ� �����ϴ� �Լ�
    void saveData() {
        // saveData�� ���� �� �������! 3������ '��� ����' ������� �� ������ �������
        // ������ �׳� ���
    }

    // �׳� �׽�Ʈ������ ��� ��ǰ, ȸ�� ���� ����ϴ� �Լ�
    void displayAllData() {
        std::cout << "\n===== ��ü ��ǰ ��� =====" << std::endl;
        for (const auto& p : products) {
            p.display();
        }
        std::cout << "\n===== ��ü ȸ�� ��� =====" << std::endl;
        for (const auto& m : members) {
            m.display();
        }
    }
};